// SPDX-License-Identifier: GPL-3.0
#include "trace.h"
#include <QProcess>
#include <QXmlStreamReader>
#include <QBuffer>
#include <QDebug>
#include <functional>
#include <QProcessEnvironment>

int Trace::loadTrace(const QString &fn, const QString &filter)
{
    fn_ = fn;
    filter_ = filter;

    QProcess proc;
    QString tshark;
    QStringList args;
    QProcessEnvironment env;
    #if (defined(Q_OS_WINDOWS))
        tshark = "tshark.exe";
    #else
        tshark = "tshark";
    #endif

    // can't handle user defined gui.column.format in preferences
    env = QProcessEnvironment::systemEnvironment();
    env.insert("WIRESHARK_CONFIG_DIR", "invalid");
    args << "-r" << fn_;
    if (!filter_.isEmpty())
        args << "-Y" << filter;
    args << "-T" << "psml";

    proc.setProcessEnvironment(env);
    proc.start(tshark, args, QProcess::ReadOnly);
    proc.waitForStarted();
    proc.waitForFinished();

    QByteArray out = proc.readAllStandardOutput();
    // qDebug() << out;
    QBuffer outbuf;
    outbuf.setBuffer(&out);
    outbuf.open(QIODevice::ReadOnly);

    QXmlStreamReader xml;
    xml.setDevice(&outbuf);

    if (!xml.readNextStartElement())
        return -1;
    if (xml.name() != QString("psml"))
        return -1;
    if (!xml.readNextStartElement())
        return -1;
    if (xml.name() != QString("structure"))
        return -1;

    if (!(xml.readNextStartElement() && xml.name() == QString("section") && xml.readNext() && xml.text() == QString("No."))) {
        qDebug("bad section, expected no");
        return -1;
    }
    xml.skipCurrentElement();

    if (!(xml.readNextStartElement() && xml.name() == QString("section") && xml.readNext() && xml.text() == QString("Time"))) {
        qDebug("bad section, expected time");
        return -1;
    }
    xml.skipCurrentElement();

    if (!(xml.readNextStartElement() && xml.name() == QString("section") && xml.readNext() && xml.text() == QString("Source"))) {
        qDebug("bad section, expected source");
        return -1;
    }
    xml.skipCurrentElement();

    if (!(xml.readNextStartElement() && xml.name() == QString("section") && xml.readNext() && xml.text() == QString("Destination"))) {
        qDebug("bad section, expected destination");
        return -1;
    }
    xml.skipCurrentElement();

    if (!(xml.readNextStartElement() && xml.name() == QString("section") && xml.readNext() && xml.text() == QString("Protocol"))) {
        qDebug("bad section, expected protocol");
        return -1;
    }
    xml.skipCurrentElement();

    if (!(xml.readNextStartElement() && xml.name() == QString("section") && xml.readNext() && xml.text() == QString("Length"))) {
        qDebug("bad section, expected length");
        return -1;
    }
    xml.skipCurrentElement();

    if (!(xml.readNextStartElement() && xml.name() == QString("section") && xml.readNext() && xml.text() == QString("Info"))) {
        qDebug("bad section, expected info");
        return -1;
    }
    xml.skipCurrentElement();

    // end of <structure>
    xml.skipCurrentElement();

    if (!xml.readNextStartElement()) {
        // empty list
        return 0;
    }
    int i = 0;
    do {
        if (xml.name() != QString("packet"))
            return -1;
        pkts_.append(Summary());

        if (!xml.readNextStartElement() || xml.name() != QString("section"))
            return -1;
        xml.readNext();
        pkts_[i].no = xml.text().toInt();
        xml.skipCurrentElement();

        if (!xml.readNextStartElement() || xml.name() != QString("section"))
            return -1;
        xml.readNext();
        pkts_[i].time = xml.text().toDouble();
        xml.skipCurrentElement();

        if (!xml.readNextStartElement() || xml.name() != QString("section"))
            return -1;
        xml.readNext();
        pkts_[i].src = xml.text().toString();
        xml.skipCurrentElement();

        if (!xml.readNextStartElement() || xml.name() != QString("section"))
            return -1;
        xml.readNext();
        pkts_[i].dst = xml.text().toString();
        xml.skipCurrentElement();

        if (!xml.readNextStartElement() || xml.name() != QString("section"))
            return -1;
        xml.readNext();
        pkts_[i].proto = xml.text().toString();
        xml.skipCurrentElement();

        if (!xml.readNextStartElement() || xml.name() != QString("section"))
            return -1;
        xml.readNext();
        // we don't care about length
        //pkts_[i].src = xml.text();
        xml.skipCurrentElement();

        if (!xml.readNextStartElement() || xml.name() != QString("section"))
            return -1;
        xml.readNext();
        pkts_[i].info = xml.text().toString();
        xml.skipCurrentElement();

        // end of <packet>
        xml.skipCurrentElement();
        i++;

    } while (xml.readNextStartElement());

    loaded_ = true;
    return 0;
}

void Trace::dump()
{
    for (int i = 0; i < pkts_.size(); i++) {
        qDebug() << pkts_[i].no << " " << pkts_[i].time << " " << pkts_[i].src << " " << pkts_[i].dst << " " << pkts_[i].proto << " " << pkts_[i].info;
    }
}

QByteArray* Trace::getPDML(int no)
{
    QProcess proc;
    QString tshark;
    QStringList args;

    #if (defined(Q_OS_WINDOWS))
        tshark = "tshark.exe";
    #else
        tshark = "tshark";
    #endif

    args << "-r" << fn_;
    args << "-Y" << (QString("frame.number == %1").arg(no));
    args << "-T" << "pdml";

    proc.start(tshark, args, QProcess::ReadOnly);
    proc.waitForStarted();
    proc.waitForFinished();

    QByteArray *out = new QByteArray(proc.readAllStandardOutput());
    return out;
}

void Trace::Node::releaseNodeHierarchy(Trace::Node *n)
{
    // get parent
    while (n->parent) {
        n = n->parent;
    }

    // recursively free on the way up
    std::function<void(Trace::Node* n)> release = [&release](Trace::Node* n) {
            if (!n->isLeaf())
                for (Trace::Node* c : n->children)
                    release(c);
            delete n;
    };

    release(n);
}

int Trace::Node::depth() const
{
    const Node* p = parent;
    int n = 0;
    while (p) {
        n++;
        p = p->parent;
    }
    return n;
}

void Trace::Node::dump(int n) const
{
    {
        auto d = qDebug().nospace();
        for (int i = 0; i < n; i++)
            d << "    ";
        d << "name=" << name << " val=" << val;
    }
    for (int i = 0; i < children.size(); i++) {
        children[i]->dump(n+1);
    }
}

bool Trace::Node::operator==(const Node& rhs) const
{
    //qDebug("node operator ===");
    return name == rhs.name && val == rhs.val;
}

static Trace::Node* parseNode(QXmlStreamReader& xml)
{
    Trace::Node *n = new Trace::Node;
    if (!xml.isStartElement()) {
        qDebug("not at start element");
        throw Trace::ParseError();
    }

    if (xml.name() == QString("packet")) {
        n->name = "root";
        n->val = "";
        n->parent = nullptr;
    } else {
        if (xml.name() != QString("proto") && xml.name() != QString("field")) {
            qDebug("not at <proto> or <field>");
            throw Trace::ParseError();
        }

        n->name = xml.attributes().value("name").toString();
        n->val = (xml.attributes().hasAttribute("showname") ? xml.attributes().value("showname") : xml.attributes().value("show")).toString();
    }

    while (xml.readNextStartElement()) {
        Trace::Node* child = parseNode(xml);
        child->parent = n;
        n->children.append(child);
    }

    return n;
}

Trace::Node* Trace::getPacket(int no)
{
    if (!cache_.contains(no)) {
        QBuffer outbuf;
        outbuf.setBuffer(getPDML(no));
        outbuf.open(QIODevice::ReadOnly);

        QXmlStreamReader xml;
        xml.setDevice(&outbuf);

        if (!xml.readNextStartElement())
            throw ParseError();
        if (xml.name() != QString("pdml"))
            throw ParseError();
        if (!xml.readNextStartElement())
            throw ParseError();
        if (xml.name() != QString("packet"))
            throw ParseError();

        cache_.insert(no, new Trace::Tree(parseNode(xml)));
    }
    return cache_[no]->root;
}

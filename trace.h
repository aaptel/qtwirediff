// SPDX-License-Identifier: GPL-3.0
#ifndef TRACE_H
#define TRACE_H

#include <QObject>
#include <QVector>
#include <QCache>
#include <QtConcurrent>

class Trace : public QObject
{
    Q_OBJECT
public:
    struct Summary {
        int no;
        double time;
        QString src;
        QString dst;
        QString proto;
        QString info;
    };

    struct Node {
        Node* parent;
        QString name;
        QString val;
        QVector<Node*> children;

        bool isLeaf() const {return children.isEmpty();}
        uint cached_hash;
        uint hash();
        void dump(int n=0);
        int depth() const {
            const Node* p = parent;
            int n = 0;
            while (p) {
                n++;
                p = p->parent;
            }
            return n;
        }
        bool operator==(const Node& rhs) const;
    };

    class ParseError : public QtConcurrent::Exception
    {
    public:
        void raise() const { throw *this; }
        ParseError *clone() const { return new ParseError(*this); }
    };

    explicit Trace(QObject *parent = nullptr);
    int loadTrace(const QString& fn);
    Node* getPacket(int no);
    void dump();
    size_t getPacketCount() {return pkts_.size();}
    const Summary& getSummary(int n) { return pkts_[n];}

    QString getFilename() const { return fn_; }
    bool isLoaded() const { return loaded_; }
    QString getFilter() const { return filter_; }
    void setFilter(const QString& s) { filter_ = s;}

private:
    QString fn_;
    QString filter_;
    bool loaded_;    
    QVector<Summary> pkts_;
    QCache<int, QByteArray> cache_;

    QByteArray* getPDML(int no);

signals:

public slots:
};


#endif // TRACE_H

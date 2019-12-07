#include "mainwindow.h"
#include <QApplication>
#include "trace.h"
#include "diff.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Trace t;
    t.loadTrace("/home/aaptel/prog/smbcmp/sample-caps/smb311-ls.pcap");
    //t.dump();

    QVector<DiffNode> diff;
    computeDiff(diff, t.getPacket(100), t.getPacket(104));
    //for (auto& it : diff) { qDebug() << it.toString(); }

    MainWindow w;
    w.show();
    w.setTraceA(&t);
    w.setTraceB(&t);
    w.setFlatDiff(&diff);

    return a.exec();
}

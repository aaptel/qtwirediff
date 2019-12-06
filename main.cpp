#include "mainwindow.h"
#include <QApplication>
#include "trace.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    Trace t;
    t.loadTrace("/home/aaptel/prog/smbcmp/sample-caps/smb311-ls.pcap");
    t.dump();
    t.getPacket(90)->dump();
    return 0;
    return a.exec();
}

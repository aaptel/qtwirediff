// SPDX-License-Identifier: GPL-3.0
#include "mainwindow.h"
#include <QApplication>
#include "trace.h"
#include "diff.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

// SPDX-License-Identifier: GPL-3.0
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    // TODO: add cli arguments (files for now)
    return a.exec();
}

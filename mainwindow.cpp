// SPDX-License-Identifier: GPL-3.0
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trace.h"
#include "keyeventfilter.h"

MainWindow * MainWindow::instance = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    instance = this;
    ui->setupUi(this);
    KeyEventFilter::install(this);

    traceLeft = new TraceView(this);
    traceRight = new TraceView(this);
    diffview = new DiffView(this);
    ui->traceSplitter->addWidget(traceLeft);
    ui->traceSplitter->addWidget(traceRight);
    ui->diffSplitter->addWidget(diffview);
    traceLeft->show();
    traceRight->show();

    connect(traceLeft, &TraceView::packetChanged, this, &MainWindow::onPacketChange);
    connect(traceRight, &TraceView::packetChanged, this, &MainWindow::onPacketChange);

}

void MainWindow::moveSelection(int dir)
{
    traceLeft->moveSelection(dir);
    traceRight->moveSelection(dir);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPacketChange(TraceView* tv)
{
    Trace::Node *left, *right;
    left = traceLeft->getLastNode();
    right = traceRight->getLastNode();

    diff.clear();
    if (left && right)
        computeDiff(diff, left, right);
    diffview->updateDiff();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About WireDiff", "WireDiff is free software licensed under the GPLv3.");
}

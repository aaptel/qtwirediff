// SPDX-License-Identifier: GPL-3.0
#include <QMessageBox>
#include <QWindow>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trace.h"
#include "keyeventfilter.h"
#include "sessionitem.h"

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
    connect(diffview, &DiffView::filterSubmitted, this, [this](QString f) {
        Q_UNUSED(f);
        onPacketChange(nullptr);
    });
    reloadSessionMenu();
}

void MainWindow::moveSelection(int dir)
{
    traceLeft->moveSelection(dir);
    traceRight->moveSelection(dir);
}

MainWindow::~MainWindow()
{
    sesList.save();
    delete ui;
}

void MainWindow::onSessionItemClick(bool checked)
{
    Q_UNUSED(checked);
    SessionItem* it = static_cast<SessionItem*>(QObject::sender());
    traceLeft->asyncOpen(it->ses_.fileA, it->ses_.filterA);
    traceRight->asyncOpen(it->ses_.fileB, it->ses_.filterB);
    diffview->setFilter(it->ses_.diffFilter);
}

void MainWindow::reloadSessionMenu()
{
    ui->menuPrevious_sessions->clear();
    for (Session& s : sesList.ses_) {
        auto it = new SessionItem(ui->menuPrevious_sessions, &s);
        ui->menuPrevious_sessions->addAction(it);
        connect(it, &SessionItem::triggered, this, &MainWindow::onSessionItemClick);
    }
}

void MainWindow::updateSession()
{
    if (traceLeft->getTrace() && traceRight->getTrace()) {
        sesList.add(traceLeft->getTrace()->getFilename(), traceLeft->getTrace()->getFilter(),
                traceRight->getTrace()->getFilename(), traceRight->getTrace()->getFilter(),
                diffview->getFilter());
        reloadSessionMenu();
    }
}

void MainWindow::onPacketChange(TraceView* tv)
{
    Q_UNUSED(tv);
    Trace::Node *left, *right;
    left = traceLeft->getLastNode();
    right = traceRight->getLastNode();

    diff.clear();
    if (left && right)
        computeDiff(diff, left, right, diffview->getFilter());
    diffview->updateDiff();

    updateSession();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About WireDiff", "WireDiff is free software licensed under the GPLv3.");
}

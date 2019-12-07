#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trace.h"
#include "tracemodel.h"
#include "difftreemodel.h"
#include "difflinedelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setFlatDiff(QVector<DiffNode>* diff)
{
    auto* m = new DiffTreeModel(this);
    m->setDiff(diff);
    ui->diffview->setItemDelegate(new DiffLineDelegate(this));
    m->tv = ui->diffview;
    ui->diffview->setModel(m);
}

void MainWindow::setTraceA(Trace* t) {
    setTrace(t, ui->traceA);
}

void MainWindow::setTraceB(Trace* t) {
    setTrace(t, ui->traceB);
}

void MainWindow::setTrace(Trace* t, QTableView* tv) {
    TraceModel *tm = new TraceModel(this);
    tm->setTrace(t);
    tv->setModel(tm);
    tv->horizontalHeader()->setVisible(true);
    tv->show();
}

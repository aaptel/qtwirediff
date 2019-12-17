// SPDX-License-Identifier: GPL-3.0
#include "traceview.h"
#include "tracemodel.h"
#include "ui_traceview.h"
#include "mainwindow.h"
#include "keyeventfilter.h"
#include <QFileDialog>
#include <QFuture>
#include <QStatusBar>

TraceView::TraceView(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TraceView)
{
    trace_ = nullptr;
    lastNode = nullptr;
    ui->setupUi(this);
    KeyEventFilter::install(this);
    KeyEventFilter::install(ui->tvTrace);
    ui->tvTrace->setSelectionBehavior (QAbstractItemView::SelectRows);
    connect(ui->btOpen, &QAbstractButton::clicked, this, &TraceView::onOpenClick);
    connect(ui->txtFilter, &QLineEdit::returnPressed, this, &TraceView::onFilterSubmit);
}

TraceView::~TraceView()
{
    delete ui;
}

void TraceView::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    int no = getPacketNo();
    MainWindow::instance->statusBar()->showMessage(QString("Loading packet %1...").arg(no));

    watcherNode = new QFutureWatcher<Trace::Node*>;
    connect(watcherNode, &QFutureWatcher<Trace::Node*>::finished, [this](){
        Trace::Node *sel = futureNode.result();
        if (!sel)
            return;

        if (sel == lastNode)
            return;

        lastNode = sel;
        MainWindow::instance->statusBar()->clearMessage();
        emit packetChanged(this);
        delete watcherNode;
    });
    futureNode = QtConcurrent::run([this, no]() {
        auto r = trace_->getPacket(no);
        return r;
    });
    watcherNode->setFuture(futureNode);
}

void TraceView::moveSelection(int dir)
{
    int row = getRow();

    auto tm = ui->tvTrace->model();
    if (!tm)
        return;

    int max = tm->rowCount()-1;
    if (row < 0)
        row = 0;
    else {
        row = qBound(0, row+dir, max);
    }
    auto sm = ui->tvTrace->selectionModel();
    if (!sm)
        return;

    sm->select(ui->tvTrace->model()->index(row, 0), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

int TraceView::getRow()
{
    auto sm = ui->tvTrace->selectionModel();
    if (!sm)
        return -1;
    auto sel = sm->selection();
    if (sel.isEmpty())
        return -1;
    return sel[0].topLeft().row();
}

int TraceView::getPacketNo()
{
    int no;
    int row = getRow();
    if (row < 0)
        row = 0;
    auto tvm = ui->tvTrace->model();
    no = tvm->index(row, 0).data().toInt();
    return no;
}

void TraceView::onOpenClick(bool checked)
{
    Q_UNUSED(checked);
    QString fn = QFileDialog::getOpenFileName(this,
                                              tr("Open trace file"), "",
                                               "All Files (*)");
    if (fn.isEmpty())
        return;

    asyncOpen(fn);
}

void TraceView::asyncOpen(QString fn, QString filter)
{
    ui->lbName->setText("Loading...");
    ui->btOpen->setEnabled(false);
    watcherTrace = new QFutureWatcher<Trace*>;

    connect(watcherTrace, &QFutureWatcher<Trace*>::finished, [fn, this](){
        Trace* oldTrace = this->trace_;
        this->trace_ = this->futureTrace.result();
        this->lastNode = nullptr;
        this->ui->tvTrace->setModel(new TraceModel(this, this));
        connect(this->ui->tvTrace->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TraceView::onSelectionChanged);
        this->ui->tvTrace->horizontalHeader()->setVisible(true);
        int colsize = 50;
        this->ui->tvTrace->setColumnWidth(0, colsize); // no
        this->ui->tvTrace->setColumnWidth(1, colsize); // time
        this->ui->tvTrace->setColumnWidth(2, colsize); // src
        this->ui->tvTrace->setColumnWidth(3, colsize); // dst
        this->ui->tvTrace->setColumnWidth(4, colsize); // proto
        this->ui->tvTrace->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
        //ui->tvTrace->setColumnWidth(5, qMax(colsize, ui->tvTrace->width()-5*colsize)); // info
        this->ui->tvTrace->show();
        this->ui->lbName->setText(fn);
        ui->btOpen->setEnabled(true);
        emit packetChanged(this);
        delete this->watcherTrace;
        delete oldTrace;
    });
    futureTrace = QtConcurrent::run([=]() {
        Trace* trace = new Trace();
        trace->loadTrace(fn, filter);
        return trace;
    });
    watcherTrace->setFuture(futureTrace);
}

void TraceView::onFilterSubmit()
{
    QString f = ui->txtFilter->text();
    if (!trace_)
        return;
    if (f == trace_->getFilter())
        return;

    asyncOpen(trace_->getFilename(), f);
}

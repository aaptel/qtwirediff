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
    ui->setupUi(this);
    KeyEventFilter::install(this);
    KeyEventFilter::install(ui->tvTrace);
    ui->tvTrace->setSelectionBehavior (QAbstractItemView::SelectRows);
    connect(ui->btOpen, &QAbstractButton::clicked, this, &TraceView::onOpen);
}

TraceView::~TraceView()
{
    delete ui;
}


void TraceView::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
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
        QThread::sleep(2);
        auto r = trace_->getPacket(no);
        QThread::sleep(2);
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

Trace::Node* TraceView::getPacket(int no)
{
    if (!trace_ || !trace_->isLoaded())
        return nullptr;

    if (no < 0) {
        no = getPacketNo();
    }

    return trace_->getPacket(no);
}

void TraceView::onOpen(bool checked)
{
    QString fn = QFileDialog::getOpenFileName(this,
                                              tr("Open trace file"), "",
                                               "All Files (*)");
    if (fn.isEmpty())
        return;

    ui->lbName->setText("Loading...");
    ui->btOpen->setEnabled(false);
    watcherTrace = new QFutureWatcher<Trace*>;

    connect(watcherTrace, &QFutureWatcher<Trace*>::finished, [fn, this](){
        this->trace_ = this->futureTrace.result();
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
    });
    futureTrace = QtConcurrent::run([=]() {
        Trace* trace = new Trace();
        trace->loadTrace(fn);
        return trace;
    });
    watcherTrace->setFuture(futureTrace);
}

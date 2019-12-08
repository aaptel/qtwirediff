#include "traceview.h"
#include "tracemodel.h"
#include "ui_traceview.h"
#include "mainwindow.h"
#include "keyeventfilter.h"
#include <QFileDialog>

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
    Trace::Node *sel = getPacket();

    if (!sel)
        return;

    if (sel == lastNode)
        return;

    lastNode = sel;
    emit packetChanged(this);
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
Trace::Node* TraceView::getPacket(int no) {
    if (!trace_ || !trace_->isLoaded())
        return nullptr;

    if (no < 0) {
        int row = getRow();
        if (row < 0)
            row = 0;
        auto tvm = ui->tvTrace->model();
        no = tvm->index(row, 0).data().toInt();
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

    ui->lbName->setText(fn);
    trace_ = new Trace();
    trace_->loadTrace(fn);
    ui->tvTrace->setModel(new TraceModel(this, this));
    connect(ui->tvTrace->selectionModel(), &QItemSelectionModel::selectionChanged, this, &TraceView::onSelectionChanged);
    ui->tvTrace->horizontalHeader()->setVisible(true);
    int colsize = 50;
    ui->tvTrace->setColumnWidth(0, colsize); // no
    ui->tvTrace->setColumnWidth(1, colsize); // time
    ui->tvTrace->setColumnWidth(2, colsize); // src
    ui->tvTrace->setColumnWidth(3, colsize); // dst
    ui->tvTrace->setColumnWidth(4, colsize); // proto
    ui->tvTrace->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    //ui->tvTrace->setColumnWidth(5, qMax(colsize, ui->tvTrace->width()-5*colsize)); // info
    ui->tvTrace->show();
    emit packetChanged(this);
}

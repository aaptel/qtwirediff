// SPDX-License-Identifier: GPL-3.0
#include "tracemodel.h"
#include "traceview.h"

TraceModel::TraceModel(QObject *parent, TraceView *tv) : QAbstractTableModel(parent), tv_(tv)
{
}

int TraceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return tv_->getTrace()->getPacketCount();
}

int TraceModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant TraceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }
    int i = index.row();
    switch (index.column()) {
    case 0:
        return tv_->getTrace()->getSummary(i).no;
    case 1:
        return tv_->getTrace()->getSummary(i).time;
    case 2:
        return tv_->getTrace()->getSummary(i).src;
    case 3:
        return tv_->getTrace()->getSummary(i).dst;
    case 4:
        return tv_->getTrace()->getSummary(i).proto;
    case 5:
        return tv_->getTrace()->getSummary(i).info;;
    }
    return QVariant();
}

QVariant TraceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("No.");
        case 1:
            return QString("Time");
        case 2:
            return QString("Source");
        case 3:
            return QString("Dest.");
        case 4:
            return QString("Proto");
        case 5:
            return QString("Info");
        }
    }
    return QVariant();
}

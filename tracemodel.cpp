#include "tracemodel.h"

TraceModel::TraceModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int TraceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return trace_->getPacketCount();
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
        return trace_->getSummary(i).no;
    case 1:
        return trace_->getSummary(i).time;
    case 2:
        return trace_->getSummary(i).src;
    case 3:
        return trace_->getSummary(i).dst;
    case 4:
        return trace_->getSummary(i).proto;
    case 5:
        return trace_->getSummary(i).info;;
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

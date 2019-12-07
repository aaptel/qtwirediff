#ifndef TRACEMODEL_H
#define TRACEMODEL_H

#include <QObject>

#include "trace.h"

class TraceModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TraceModel(QObject *parent = 0);

    void setTrace(Trace* trace) { trace_ = trace;}

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

private:
    Trace* trace_;
};

#endif // TRACEMODEL_H

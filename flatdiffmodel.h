#ifndef FLATDIFFMODEL_H
#define FLATDIFFMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include "diff.h"

class FlatDiffModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FlatDiffModel(QObject *parent = nullptr);
    ~FlatDiffModel();

    void setDiff(QVector<DiffNode>* diff);
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    void setupModelData(const QStringList &lines, DiffNode *parent);

    QVector<DiffNode>* diff_;
};

#endif // FLATDIFFMODEL_H

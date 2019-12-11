// SPDX-License-Identifier: GPL-3.0
#ifndef FLATDIFFMODEL_H
#define FLATDIFFMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QTreeView>

#include "diff.h"

struct DiffItem {
    DiffItem *parent;
    DiffNode *node;
    QVector<DiffItem*> children;

    int row() const
    {
        if (parent)
            return parent->children.indexOf(const_cast<DiffItem*>(this));
        return 0;
    }

    bool holdsChanges(bool checkSelf = false)
    {
        if (checkSelf && node->type != 0)
            return true;
        for (auto& c : children) {
            if (c->holdsChanges(true))
                return true;
        }
        return false;
    }
};

class DiffTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit DiffTreeModel(QObject *parent = nullptr);
    ~DiffTreeModel();
    void updateDiff();
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QTreeView *tv;
private:
    void setupModelData(const QStringList &lines, DiffNode *parent);

    DiffItem *root;
};

#endif // FLATDIFFMODEL_H

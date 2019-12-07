#include "flatdiffmodel.h"

struct DiffItem {
    DiffItem *parent;
    DiffNode *node;
    QVector<DiffItem*> children;



};


FlatDiffModel::FlatDiffModel(QObject *parent)
    : QAbstractItemModel(parent)
{

}

FlatDiffModel::~FlatDiffModel()
{

}

void FlatDiffModel::setDiff(QVector<DiffNode>* diff)
{
    diff_ = diff;

    for (int i = 1; i < diff->size(); i++) {

    }
}

int FlatDiffModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant FlatDiffModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    DiffNode *item = static_cast<DiffNode*>(index.internalPointer());
    return item->toString();
}

Qt::ItemFlags FlatDiffModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant FlatDiffModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return "Text";
        default:
            return "foo";
        }
    }
    return QVariant();
}

/* TODO: look into delegates to color rows properly */
QModelIndex FlatDiffModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent) || row >= diff_->size())
        return QModelIndex();

    return createIndex(row, column, &(*diff_)[row]);
}

QModelIndex FlatDiffModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int FlatDiffModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return diff_->size();
}

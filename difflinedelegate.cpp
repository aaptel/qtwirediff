#include "difflinedelegate.h"
#include "flatdiffmodel.h"
#include <QPainter>

DiffLineDelegate::DiffLineDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{

}

void DiffLineDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    if(1){QStyledItemDelegate::paint(painter, option, index); return;}
    if (option.state & QStyle::State_Selected)
       painter->fillRect(option.rect, option.palette.highlight());
    painter->drawText(option.rect, "foo");
}


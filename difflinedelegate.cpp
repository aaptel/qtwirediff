// SPDX-License-Identifier: GPL-3.0
#include "difflinedelegate.h"
#include "difftreemodel.h"
#include <QPainter>

DiffLineDelegate::DiffLineDelegate(QWidget *parent) : QStyledItemDelegate(parent)
{

}

void DiffLineDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    DiffItem *it = static_cast<DiffItem*>(index.internalPointer());
    bool expanded = static_cast<const DiffTreeModel*>(index.model())->tv->isExpanded(index);
    QBrush b;
    bool fill = true;

    //if(1){QStyledItemDelegate::paint(painter, option, index); return;}

    if (option.state & QStyle::State_Selected) {
        b = option.palette.highlight();
    } else if (it->node->type < 0) {
        b = QBrush(QColor(0, 168, 107));
    } else if (it->node->type > 0) {
        b = QBrush(QColor(255, 127, 127));
    } else if (!expanded && it->holdsChanges()) {
            b = QBrush(QColor(235,225,136));
    } else {
        fill = false;
    }

    if (fill) {
        painter->fillRect(option.rect, b);
    }
    painter->drawText(option.rect, it->node->p->val);
}


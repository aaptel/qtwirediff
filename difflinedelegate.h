// SPDX-License-Identifier: GPL-3.0
#ifndef DIFFLINEDELEGATE_H
#define DIFFLINEDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class QBrush;

class DiffLineDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    DiffLineDelegate(QWidget *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

};


#endif // DIFFLINEDELEGATE_H

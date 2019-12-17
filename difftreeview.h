// SPDX-License-Identifier: GPL-3.0
#ifndef DIFFTREEVIEW_H
#define DIFFTREEVIEW_H

#include <QTreeView>
#include <QObject>

class DiffTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit DiffTreeView(QWidget *parent = 0);
    DiffTreeView();
    void updateDiff();
};

#endif // DIFFTREEVIEW_H

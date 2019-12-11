// SPDX-License-Identifier: GPL-3.0
#ifndef DIFFVIEW_H
#define DIFFVIEW_H

#include <QTreeView>
#include <QObject>

class DiffView : public QTreeView
{
    Q_OBJECT
public:
    explicit DiffView(QWidget *parent = 0);
    DiffView();
    void updateDiff();
};

#endif // DIFFVIEW_H

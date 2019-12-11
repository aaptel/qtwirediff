// SPDX-License-Identifier: GPL-3.0
#include "mainwindow.h"
#include "diffview.h"
#include "difftreemodel.h"
#include "difflinedelegate.h"
#include "keyeventfilter.h"

DiffView::DiffView(QWidget *parent) : QTreeView(parent)
{
    KeyEventFilter::install(this);
}

void DiffView::updateDiff()
{
    auto* m = new DiffTreeModel(this);
    m->updateDiff();
    setItemDelegate(new DiffLineDelegate(this));
    m->tv = this;
    setModel(m);
}

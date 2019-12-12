// SPDX-License-Identifier: GPL-3.0
#include "mainwindow.h"
#include "diffview.h"
#include "difftreemodel.h"
#include "difflinedelegate.h"
#include "keyeventfilter.h"

DiffView::DiffView(QWidget *parent) : QTreeView(parent)
{
    KeyEventFilter::install(this);
    setModel(new DiffTreeModel(this));
    setItemDelegate(new DiffLineDelegate(this));
}

void DiffView::updateDiff()
{
    static_cast<DiffTreeModel*>(model())->updateDiff();
}

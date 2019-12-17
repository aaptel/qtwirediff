// SPDX-License-Identifier: GPL-3.0
#include "mainwindow.h"
#include "difftreeview.h"
#include "difftreemodel.h"
#include "difflinedelegate.h"
#include "keyeventfilter.h"

DiffTreeView::DiffTreeView(QWidget *parent) : QTreeView(parent)
{
    KeyEventFilter::install(this);
    setModel(new DiffTreeModel(this));
    setItemDelegate(new DiffLineDelegate(this));
}

void DiffTreeView::updateDiff()
{
    static_cast<DiffTreeModel*>(model())->updateDiff();
}

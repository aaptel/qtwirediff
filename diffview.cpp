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
    auto oldModel = model();
    auto oldDeleg = itemDelegate();

    auto* newModel = new DiffTreeModel(this);
    newModel->updateDiff();
    setItemDelegate(new DiffLineDelegate(this));
    setModel(newModel);

    // release previous ressources (including all DiffItems)
    if (oldModel)
        oldModel->deleteLater();
    if (oldDeleg)
        oldDeleg->deleteLater();
}

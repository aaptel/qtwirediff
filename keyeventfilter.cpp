// SPDX-License-Identifier: GPL-3.0
#include "keyeventfilter.h"
#include "mainwindow.h"
#include <QKeyEvent>

bool KeyEventFilter::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        switch (key->key()) {
        case Qt::Key_PageUp:
            MainWindow::instance->moveSelection(-1);
            break;
        case Qt::Key_PageDown:
            MainWindow::instance->moveSelection(+1);
            break;
        default:
            return QObject::eventFilter(obj, event);
        }
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
    return false;
}

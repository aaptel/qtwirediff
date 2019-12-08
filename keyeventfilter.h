#ifndef KEYEVENTFILTER_H
#define KEYEVENTFILTER_H

#include <QObject>
#include <QEvent>
#include <QWidget>

class KeyEventFilter : public QObject
{
    Q_OBJECT
public:
    static void install(QWidget *w)
    {
        w->installEventFilter(new KeyEventFilter(w));
    }
protected:
    KeyEventFilter(QObject *parent = 0) : QObject(parent) {}
    bool eventFilter(QObject* obj, QEvent* event);
};

#endif // KEYEVENTFILTER_H

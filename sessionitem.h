#ifndef SESSIONITEM_H
#define SESSIONITEM_H
#include "session.h"
#include <QObject>
#include <QAction>
class SessionItem : public QAction
{
    Q_OBJECT
    public:
    Session ses_;
    SessionItem(QObject* p=0, Session* ses=0);
};

#endif // SESSIONITEM_H

#include "sessionitem.h"
#include <QFileInfo>


SessionItem::SessionItem(QObject* p, Session* ses) : QAction(p), ses_(*ses)
{
     setText(QFileInfo(ses_.fileA).fileName()+" | "+QFileInfo(ses_.fileB).fileName());
}

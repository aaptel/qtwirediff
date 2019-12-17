#include "sessionitem.h"
#include <QFileInfo>


SessionItem::SessionItem(QWidget* p, Session* ses) : QAction(p), ses_(*ses)
{
     setText(QFileInfo(ses_.fileA).fileName()+" | "+QFileInfo(ses_.fileB).fileName());
}

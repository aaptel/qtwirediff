#include "difffilterlist.h"

DiffFilterList::DiffFilterList(QString filter)
{
    const QStringList& slist = filter.split(",");
    if (slist.isEmpty())
        return;
    for (const QString& s : slist) {
        if (s.isEmpty())
            continue;
        if (s[0] == '!' && s.length() >= 2) {
            hideId_.append(s.mid(1));
        } else {
            showId_.append(s);
        }
    }
}

bool DiffFilterList::isWhitelisted(const DiffNode& n) const
{
    return showId_.isEmpty() || showId_.contains(n.p->name);
}

bool DiffFilterList::isBlacklisted(const DiffNode& n) const
{
    return hideId_.contains(n.p->name);
}

bool DiffFilterList::isNodeAccepted(const DiffNode& n) const
{
    if ((showId_.isEmpty() || showId_.contains(n.p->name)) && !hideId_.contains(n.p->name)) {
        return true;
    }
    return false;
}

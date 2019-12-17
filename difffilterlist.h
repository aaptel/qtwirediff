#ifndef DIFFFILTERLIST_H
#define DIFFFILTERLIST_H

#include <QVector>
#include "diff.h"

struct DiffNode;

class DiffFilterList
{
public:
    DiffFilterList(QString);
    bool isNodeAccepted(const DiffNode& n) const;
    bool isWhitelisted(const DiffNode& n) const;
    bool isBlacklisted(const DiffNode& n) const;
private:
    QVector<QString> showId_;
    QVector<QString> hideId_;
};

#endif // DIFFFILTERLIST_H

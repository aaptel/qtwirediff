#ifndef DIFF_H
#define DIFF_H

#include <QVector>
#include "trace.h"


struct DiffNode {
    int type;
    Trace::Node *p;
    bool operator==(const DiffNode& rhs) const {
        return *p == *rhs.p;
    }
    QString toString() const {
        int n = p->depth();
        QString s;
        s += type < 0 ? '-' : type > 0 ? '+' : ' ';
        for (int i = 0; i < n; i++)
            s += "    ";
        s += p->val;
        return s;
    }
};

void computeDiff(QVector<DiffNode>& res, Trace::Node* na, Trace::Node* nb);

#endif // DIFF_H

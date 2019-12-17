// SPDX-License-Identifier: GPL-3.0
#include "diff.h"
#include "dtl/dtl.hpp"
#include <vector>

static void flattenForDiff(std::vector<DiffNode>& vec, Trace::Node *n, const DiffFilterList& filt, bool whiteParent = false)
{
    DiffNode dn = {0, n};
    bool white = filt.isWhitelisted(dn);
    bool black = filt.isBlacklisted(dn);

    if (black)
        return;

    if (!n->parent) {
        vec.push_back(dn);
    } else if (white || whiteParent) {
        vec.push_back(dn);
        whiteParent = true;
    }

    for (int i = 0; i < n->children.size(); i++) {
        flattenForDiff(vec, n->children[i], filt, whiteParent);
    }
}

void computeDiff(QVector<DiffNode>& res, Trace::Node* na, Trace::Node* nb, const DiffFilterList& filt)
{
    std::vector<DiffNode> a, b;
    flattenForDiff(a, na, filt);
    flattenForDiff(b, nb, filt);

    dtl::Diff<DiffNode> diff(a, b);
    diff.compose();

    auto ses_v = diff.getSes().getSequence();
    for (auto& it : ses_v) {
        auto& elem = it.first;
        auto& info = it.second;
        elem.type = info.type;
        res.append(elem);
    }
}

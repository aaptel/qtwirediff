#include "diff.h"
#include "dtl/dtl.hpp"
#include <vector>

static void flattenForDiff(std::vector<DiffNode>& vec, Trace::Node *n)
{
    vec.push_back({0,n});
    for (int i = 0; i < n->children.size(); i++) {
        flattenForDiff(vec, n->children[i]);
    }
}

void computeDiff(QVector<DiffNode>& res, Trace::Node* na, Trace::Node* nb)
{
    std::vector<DiffNode> a, b;
    flattenForDiff(a, na);
    flattenForDiff(b, nb);

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

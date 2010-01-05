#include "ConjugateMethod.h"

#include <sampler/GraphView.h>

using std::vector;

//FIXME: Isn't this somewhere else?
vector<ConjugateDist> getChildDist(GraphView const *gv)
{
    vector<ConjugateDist> ans;
    vector<StochasticNode const *> const &child = gv->stochasticChildren();
    for (unsigned int i = 0; i < child.size(); ++i) {
	ans.push_back(getDist(child[i]));
    }
    return ans;
}

ConjugateMethod::ConjugateMethod(GraphView const *gv)
    : _target_dist(getDist(gv->nodes()[0])),
      _child_dist(getChildDist(gv))
{}

ConjugateMethod::~ConjugateMethod()
{}

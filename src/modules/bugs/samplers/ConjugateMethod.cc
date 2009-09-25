#include "ConjugateMethod.h"

#include <sampler/Updater.h>

using std::vector;

//FIXME: Isn't this somewhere else?
vector<ConjugateDist> getChildDist(Updater const *updater)
{
    vector<ConjugateDist> ans;
    vector<StochasticNode const *> const &child = updater->stochasticChildren();
    for (unsigned int i = 0; i < child.size(); ++i) {
	ans.push_back(getDist(child[i]));
    }
    return ans;
}

ConjugateMethod::ConjugateMethod(Updater const *updater)
    : _target_dist(getDist(updater->nodes()[0])),
      _child_dist(getChildDist(updater))
{}

ConjugateMethod::~ConjugateMethod()
{}

#include "ConjugateMethod.h"

#include <sampler/GraphView.h>
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <module/ModuleError.h>

#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;

namespace bugs {

ConjugateDist getDist(StochasticNode const *snode)
{
    static map<string, ConjugateDist> dist_table;
    if (dist_table.empty()) {
	dist_table["dbern"] = BERN;
	dist_table["dbeta"] = BETA;
	dist_table["dbin"] = BIN;
	dist_table["dcat"] = CAT;
	dist_table["dchisq"] = CHISQ;
	dist_table["ddexp"] = DEXP;
	dist_table["ddirch"] = DIRCH;
	dist_table["dexp"] = EXP;
	dist_table["dgamma"] = GAMMA;
	dist_table["dlnorm"] = LNORM;
	dist_table["dlogis"] = LOGIS;
	dist_table["dmnorm"] = MNORM;
	dist_table["dmulti"] = MULTI;
	dist_table["dnegbin"] = NEGBIN;
	dist_table["dnorm"] = NORM;
	dist_table["dpar"] = PAR;
	dist_table["dpois"] = POIS;
	dist_table["dt"] = T;
	dist_table["dunif"] = UNIF;
	dist_table["dweib"] = WEIB;
	dist_table["dwish"] = WISH;
    }
  
    string const &name = snode->distribution()->name();
    map<string, ConjugateDist>::iterator p(dist_table.find(name));

    if (p == dist_table.end())
	return OTHERDIST;
    else
	return p->second;
}


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
      _child_dist(getChildDist(gv)),
      _gv(gv)
{
    if (gv->nodes().size() > 1)
	throwLogicError("Multiple sample nodes in ConjugateMethod");
}

ConjugateMethod::~ConjugateMethod()
{}

}

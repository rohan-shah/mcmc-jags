#include "PDTraceFactory.h"
#include "PDTrace.h"

#include <model/BUGSModel.h>
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

#include <set>

using std::set;
using std::string;
using std::vector;

namespace jags {
namespace dic {

    Monitor *PDTraceFactory::getMonitor(string const &name,
					Range const &range,
					BUGSModel *model,
					string const &type,
					string &msg)
    {
	if (name != "pD") 
	    return 0;
	if (type != "trace")
	    return 0;
	if (model->nchain() < 2) {
	    msg = "at least two chains are required for a pD trace monitor";
	    return 0;
	}
	if (!isNULL(range)) {
	    msg = "cannot monitor a subset of pD";
	    return 0;
	}

	vector<StochasticNode const *> observed_nodes;
	vector<StochasticNode *> const &snodes = model->stochasticNodes();
	for (unsigned int i = 0; i < snodes.size(); ++i) {
	    if (snodes[i]->isFixed()) {
		observed_nodes.push_back(snodes[i]);
	    }
	    if (!isSupportFixed(snodes[i])) {
		msg = "pD is infinite because at least one observed node does not have fixed support";
		return 0;
	    }
	}
	if (observed_nodes.empty()) {
	    msg = "there are no observed nodes";
	    return 0;
	}

	unsigned int nchain = model->nchain();
	vector<RNG*> rngs;
	for (unsigned int i = 0; i < nchain; ++i) {
	    rngs.push_back(model->rng(i));
	}

	Monitor *m  = new PDTrace(observed_nodes, rngs, 10);
	m->setName("pD");
	m->setElementNames(vector<string>(1,"pD"));
	return m;
    }

    string PDTraceFactory::name() const
    {
	return "dic::PDTrace";
    }

}}

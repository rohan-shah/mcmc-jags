#include "PDMonitorFactory.h"
#include "PDMonitor.h"
#include "PoptMonitor.h"
#include "KLTab.h"
#include "CalKLExact.h"
#include "CalKLApprox.h"

#include <model/BUGSModel.h>
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>

#include <set>

using std::set;
using std::string;
using std::vector;

namespace dic {

    static KL const *findKL(string const &name) {
	static KLTab _kltab;
	return _kltab.find(name);
    }

    Monitor *PDMonitorFactory::getMonitor(string const &name,
					  Range const &range,
					  BUGSModel *model,
					  string const &type)
    {
	if (model->nchain() < 2)
	    return 0;

	if ((name != "pD" && name != "popt") || !isNULL(range))
	    return 0;
	
	if (type != "mean")
	    return 0;

	vector<StochasticNode const *> observed_nodes;
	vector<StochasticNode *> const &snodes = model->stochasticNodes();
	for (unsigned int i = 0; i < snodes.size(); ++i) {
	    if (snodes[i]->isObserved()) {
		observed_nodes.push_back(snodes[i]);
	    }
	    if (!isSupportFixed(snodes[i])) {
		return 0;
	    }
	}
	if (observed_nodes.empty())
	    return 0;

	unsigned int nchain = model->nchain();
	vector<RNG*> rngs;
	for (unsigned int i = 0; i < nchain; ++i) {
	    rngs.push_back(model->rng(i));
	}

	vector<CalKL*> calkl;
	for (unsigned int i = 0; i < observed_nodes.size(); ++i) {
	    
	    StochasticNode const *snode = observed_nodes[i];
	    KL const *kl = findKL(snode->distribution()->name());
	    if (kl) {
		calkl.push_back(new CalKLExact(snode, kl));
	    }
	    else {
		calkl.push_back(new CalKLApprox(snode, rngs, 10));
	    }
	}

	Monitor *m = 0;
	if (name =="pD") {
	    m = new PDMonitor(observed_nodes, calkl);
	}
	else if (name == "popt") {
	    m = new PoptMonitor(observed_nodes, calkl);
	}
	if (m) {
	    m->setName(name);
	    vector<string> onames(observed_nodes.size());
	    for (unsigned int i = 0; i < observed_nodes.size(); ++i) {
		onames[i] = model->symtab().getName(observed_nodes[i]);
	    }
	    m->setElementNames(onames);
	}
	return m;
    }

    string PDMonitorFactory::name() const
    {
	return "dic::pdfactory";
    }
}

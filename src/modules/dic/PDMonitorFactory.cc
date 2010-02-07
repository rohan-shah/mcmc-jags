#include "PDMonitorFactory.h"
#include "DefaultPDMonitor.h"
#include "KLPDMonitor.h"
#include "KLTab.h"

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
	if (type != "pD" || model->nchain() < 2)
	    return 0;
	
	Node const *node = model->getNode(name, range);
	StochasticNode const *snode = asStochastic(node);
	if (!snode)
	    return 0;
	
	Monitor *m = 0;
	if (isSupportFixed(snode)) {
	    
	    KL const *kl = findKL(snode->distribution()->name());
	    if (kl) {
		m = new KLPDMonitor(snode, kl);
	    }
	}
	else {

	    unsigned int nchain = model->nchain();
	    vector<RNG*> rngs;
	    for (unsigned int i = 0; i < nchain; ++i) {
		rngs.push_back(model->rng(i));
	    }
	    return new DefaultPDMonitor(snode, rngs, 10);
	}
	if (m) {
	    m->setName(name + print(range));
	    m->setElementNames(vector<string>(1, name + print(range)));
	}
	return m;
    }

}

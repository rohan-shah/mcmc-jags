#include "DevianceMonitorFactory.h"
#include "DevianceMean.h"
#include "DevianceTrace.h"

#include <model/BUGSModel.h>
#include <graph/StochasticNode.h>

#include <set>

using std::set;
using std::string;
using std::vector;

namespace dic {

    Monitor *DevianceMonitorFactory::getMonitor(string const &name, 
						Range const &range,
						BUGSModel *model,
						string const &type,
						string &msg)
    {

	if (type != "mean" && type != "trace")
	    return 0;
	if (name != "deviance")
	    return 0;
	if (!isNULL(range)) {
	    msg = "cannot monitor a subset of deviance";
	    return 0;
	}
	
	vector<StochasticNode *> const &snodes = model->stochasticNodes();
	vector<StochasticNode const *> observed_snodes;
	for (unsigned int i = 0; i < snodes.size(); ++i) {
	    if (snodes[i]->isObserved()) {
		observed_snodes.push_back(snodes[i]);
	    }
	}
	if (observed_snodes.empty())
	    return 0;

	Monitor *m = 0;
	if (type == "mean") {
	    m = new DevianceMean(observed_snodes);
	    m->setName(name);
	    vector<string> onames(observed_snodes.size());
	    for (unsigned int i = 0; i < observed_snodes.size(); ++i) {
		onames[i] = model->symtab().getName(observed_snodes[i]);
	    }
	    m->setElementNames(onames);
	}
	else if (type == "trace") {
	    m = new DevianceTrace(observed_snodes);
	    m->setName("deviance");
	    m->setElementNames(vector<string>(1,"deviance"));
	}
	return m;
    }

    string DevianceMonitorFactory::name() const
    {
	return "dic::Deviance";
    }
}

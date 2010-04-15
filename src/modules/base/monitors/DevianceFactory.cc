#include "DevianceFactory.h"
#include "Deviance.h"

#include <model/BUGSModel.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <sarray/RangeIterator.h>

using std::set;
using std::string;
using std::vector;

namespace base {

    Monitor *DevianceFactory::getMonitor(string const &name,
					 Range const &range,
					 BUGSModel *model,
					 string const &type)
    {
	if (type != "trace")
	    return 0;
	if (name != "deviance")
	    return 0;
	if (!isNULL(range))
	    return 0;

	vector<StochasticNode *> const &snodes = model->stochasticNodes();
	vector<StochasticNode const *> observed_snodes;
	for (unsigned int i = 0; i < snodes.size(); ++i) {
	    if (snodes[i]->isObserved()) {
		observed_snodes.push_back(snodes[i]);
	    }
	}
	if (observed_snodes.empty())
	    return 0;

	Monitor *devmon = new Deviance(observed_snodes);
	
	//Set name attributes 
	devmon->setName("deviance");
	devmon->setElementNames(vector<string>(1,"deviance"));
	
	return devmon;
    }

    string DevianceFactory::name() const
    {
	return "base::Deviance";
    }	
}

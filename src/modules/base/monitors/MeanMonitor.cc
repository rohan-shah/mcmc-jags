#include <config.h>
#include <graph/Node.h>

#include <algorithm>

#include "MeanMonitor.h"

using std::vector;
using std::string;

namespace jags {
namespace base {

    MeanMonitor::MeanMonitor(Node const *node)
	: Monitor("mean", node), 
	  _values(node->nchain(), vector<double>(node->length(), 0)),
	  _n(0)
    {
	
    }
    
    void MeanMonitor::update()
    {
	Node const *snode = nodes()[0];
	unsigned int nchain = _values.size();

	_n++;
	for (unsigned int ch = 0; ch < nchain; ++ch) {
	    vector<double> &rmean  = _values[ch];
	    double const *value = snode->value(ch);
	    for (unsigned int i = 0; i < snode->length(); ++i) {
		rmean[i] -= (rmean[i] - value[i])/_n;
	    }
	}
    }

    vector<double> const &MeanMonitor::value(unsigned int chain) const
    {
	return _values[chain];
    }

    vector<unsigned int> MeanMonitor::dim() const
    {
	return nodes()[0]->dim();
    }

    bool MeanMonitor::poolChains() const
    {
	return false;
    }

    bool MeanMonitor::poolIterations() const
    {
	return false;
    }

}}

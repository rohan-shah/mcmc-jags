#include "DevianceMonitor.h"

#include <graph/StochasticNode.h>

#include <stdexcept>

using std::vector;
using std::logic_error;
using std::string;

namespace dic {

    DevianceMonitor::DevianceMonitor(StochasticNode const *snode, 
				     unsigned int start, unsigned int thin)
	: Monitor("deviance", snode, start, thin), _values(snode->nchain()), 
	  _snode(snode)
    {
    }

    unsigned int DevianceMonitor::nchain() const
    {
      return 1;
    }

    vector<unsigned int> DevianceMonitor::dim() const
    {
	vector<unsigned int> d(2);
        d[0] = 1;
	d[1] = niter();
	return d;
    }
 
    vector<double> const &DevianceMonitor::value(unsigned int chain) const
    {
	return _values;
    }

    void DevianceMonitor::doUpdate()
    {
        double dev = 0;
	unsigned int nchain = _snode->nchain();

	for (unsigned int ch = 0; ch < nchain; ++ch) {
	  dev -= 2* _snode->logDensity(ch);
	}
	dev /= nchain;

	_values.push_back(dev);
    }

    void DevianceMonitor::reserve(unsigned int niter)
    {
	unsigned int N = 1 + niter / thin();
	_values.reserve(_values.size() + N);
    }
    
    SArray DevianceMonitor::dump() const
    {
	SArray ans(dim());
	ans.setValue(_values);

	vector<string> names(2);
	names[1] = "iteration";
	
	ans.setDimNames(names);
	return(ans);
    }
}

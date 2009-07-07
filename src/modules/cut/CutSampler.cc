#include <config.h>

using std::vector;
using std::set;

static vector<StochasticNode*>
appendChildren(vector<StochasticNode*> nodes, Graph const &graph)
{
    set<StochasticNode*> children;
    Sampler::getStochasticChildren(nodes, graph, children);
    for (set<StochasticNode*>::const_iterator p = children.begin();
	 p != children.end(); ++p)
    {
	nodes.push_back(*p);
    }
    return nodes;
}

namespace cut {

    CutSampler::CutSampler(vector<StochasticNode*> const &cutnodes,
			   Graph const &graph)
	: Sampler(appendChildren(cutnodes), graph), _ncut(cutnodes.size())
    {
	for (unsigned int ch = 0; ch < cutnodes[0]->nchain(); ++ch) {
	    _methods.push_back(new CutMethod(this));
	}
    }

    CutSampler::~CutSampler()
    {
	for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	    delete _methods[ch];
	}
    }

    unsigned int CutSampler::ncut() const
    {
	return _ncut;
    }

    void CutSampler::update(std::vector<RNG*> const &rngs)
    {
	for (unsigned int ch = 0; ch < rngs.size(); ++ch) {
	    _methods[ch]->update(rngs[ch]);
	}
    }

    bool CutSampler::adaptOff()
    {
	bool pass = true;
	for (unsigned int ch = 0; ch < _methods.size(); ++ch) {
	    if (!_methods[ch]->adaptOff())
		pass = false;
	}
	return pass;
    }

    bool CutSampler::isAdaptive() const
    {
	return true;
    }

    string CutSampler::name() const
    {
	return "CutSampler";
    }

    bool CutSampler::cansample(vector<StochasticNode*> const &cutnodes, 
			       Graph const &graph)
    {
	//Check that unobserved stochastic children within the graph
	//are all continuous with fixed support

	set<StochasticNode*> children;
	Sampler::getStochasticChildren(cutnodes, graph, children);
	for (set<StochasticNode*>::const_iterator p = children.begin();
	     p != children.end(); ++p)
	{
	    if (!(*p)->isObserved()) {
		if ((*p)->isDiscreteValued())
		    return false;
		if (!(*p)->isSupportFixed())
		    return false;
	    }
	}
	return true;
    }

}

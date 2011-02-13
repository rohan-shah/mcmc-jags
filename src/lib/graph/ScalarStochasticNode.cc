#include <config.h>
#include <graph/ScalarStochasticNode.h>
#include <distribution/DistError.h>
#include <distribution/ScalarDist.h>
#include <util/nainf.h>

#include <vector>
#include <string>

using std::vector;
using std::string;

ScalarStochasticNode::ScalarStochasticNode(ScalarDist const *dist, 
					   vector<Node const *> const &params,
					   Node const *lower, Node const *upper)
    : StochasticNode(vector<unsigned int>(1,1), dist, params, lower, upper), 
      _dist(dist)
{
    for (unsigned int i = 0; i < params.size(); ++i) {
	if (params[i]->length() != 1) {
	    throw DistError(dist, "Invalid non-scalar parameter");
	}
    }
}

double ScalarStochasticNode::logDensity(unsigned int chain, PDFType type) const
{
    if(!_dist->checkParameterValue(_parameters[chain]))
	return JAGS_NEGINF;
    
    return _dist->logDensity(_data[chain], type, _parameters[chain], 
			     lowerLimit(chain), upperLimit(chain));
}

void ScalarStochasticNode::deterministicSample(unsigned int chain)
{
    _data[chain] = _dist->typicalValue(_parameters[chain], 
				       lowerLimit(chain), upperLimit(chain));
}

void ScalarStochasticNode::randomSample(RNG *rng, unsigned int chain)
{
    _data[chain] = _dist->randomSample(_parameters[chain], lowerLimit(chain),
				       upperLimit(chain), rng);
}  

void ScalarStochasticNode::truncatedSample(RNG *rng, unsigned int chain,
					   double const *lb, double const *ub)
{
    double const *l = lowerLimit(chain);
    if (lb) {
	if (l == 0 || (l && (*lb < *l)))
	    l = lb;
    }
    double const *u = upperLimit(chain);
    if (ub) {
	if (u == 0 || (u && (*ub > *u)))
	    u = ub;
    }
    _data[chain] = _dist->randomSample(_parameters[chain], l, u, rng);
}  

bool ScalarStochasticNode::checkParentValues(unsigned int chain) const
{
    return _dist->checkParameterValue(_parameters[chain]);
}

bool isBounded(ScalarStochasticNode const *node)
{
    return node->lowerBound() || node->upperBound();
}

StochasticNode * 
ScalarStochasticNode::clone(vector<Node const *> const &parameters,
			    Node const *lower, Node const *upper) const
{
    return new ScalarStochasticNode(_dist, parameters, lower, upper);
}

unsigned int ScalarStochasticNode::df() const
{
    return _dist->df();
}

void ScalarStochasticNode::sp(double *lower, double *upper, unsigned int length,
			      unsigned int chain) const
{
    *lower = _dist->l(_parameters[chain]);
    *upper = _dist->u(_parameters[chain]);
}

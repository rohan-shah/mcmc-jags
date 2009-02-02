#include <config.h>
#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/NodeError.h>
#include <distribution/Distribution.h>
#include <util/nainf.h>
#include <util/dim.h>

#include <vector>
#include <stdexcept>
#include <string>
#include <stdexcept>
#include <cmath>

using std::vector;
using std::string;
using std::runtime_error;
using std::logic_error;

static vector<unsigned int> mkDim(Distribution const *dist, 
				  vector<Node const *> const &parents)
{
    /* 
       Calculates dimension of stochastic node as a function of its
       parents
    */

    if (parents.size() != dist->npar()) {
	throw runtime_error(string("Incorrect number of parameters for ") +
			    "distribution " + dist->name());
    }

    vector<vector<unsigned int> > parameter_dims(parents.size());
    for (unsigned long j = 0; j < parents.size(); ++j) {
	parameter_dims[j] = parents[j]->dim();
    }
    if (!dist->checkParameterDim(parameter_dims)) {
	throw runtime_error(string("Non-conforming parameters for ") +
			    "distribution " + dist->name());
    }
    return dist->dim(parameter_dims);
}

static vector<Node const *> mkParents(vector<Node const *> const &parameters, 
				      Node const *lower, Node const *upper)
{
    //Add bounds to vector of parents, if they are non-zero
    vector<Node const *> parents = parameters;
    if (lower) {
	parents.push_back(lower);
    }
    if (upper) {
	parents.push_back(upper);
    }
    return parents;
}

static vector<vector<unsigned int> > const &
mkParameterDims(vector<Node const *> const &parameters) {
    vector<vector<unsigned int> > dims(parameters.size());
    for (unsigned int j = 0; j < parameters.size(); ++j) {
        dims[j] = parameters[j]->dim();
    }
    return getUnique(dims);
}

StochasticNode::StochasticNode(Distribution const *dist, 
			       vector<Node const *> const &parameters,
			       Node const *lower, Node const *upper)
    : Node(mkDim(dist, parameters), mkParents(parameters, lower, upper)), 
      _dist(dist), _parameters(nchain()), _dims(mkParameterDims(parameters)),
      _lower(lower), _upper(upper), _observed(false)
{
 
    if (parameters.size() != _dist->npar()) {
	throw NodeError(this, "Incorrect number of parameters for distribution");
    }
    if (!_dist->checkParameterDim(_dims)) {
	throw NodeError(this,"Invalid parameter dimensions for distribution");
    }
    if (_dist->dim(_dims) != dim()) {
	throw NodeError(this, "Dimension mismatch between parameters and Node");
    }

    //check boundaries
    if ((lower && lower->dim() != dim()) || 
	(upper && upper->dim() != dim()))
    {
	throw NodeError(this,"Dimension mismatch when setting bounds");
    }
  
    if (!_dist->canBound() && (lower || upper)) {
	throw runtime_error(string("distribution " + dist->name() +
				   " cannot be bounded: "));
    }

    //Set up parameter vectors 
    for (unsigned int n = 0; n < nchain(); ++n) {
	_parameters[n].reserve(parameters.size());
	for (unsigned int i = 0; i < parameters.size(); ++i) {
	    _parameters[n].push_back(parameters[i]->value(n));
	}
    }

    for (unsigned int i = 0; i < parents().size(); ++i) {
	parents()[i]->addChild(this);
    }
}

StochasticNode::~StochasticNode()
{
    for (unsigned int i = 0; i < parents().size(); ++i) {
	parents()[i]->removeChild(this);
    }
}

Node const *StochasticNode::lowerBound() const
{
    return _lower;
}

Node const *StochasticNode::upperBound() const
{
    return _upper;
}

double const *StochasticNode::lowerLimit(unsigned int chain) const
{
    return _lower ? _lower->value(chain) : 0;
}

double const *StochasticNode::upperLimit(unsigned int chain) const
{
    return _upper ? _upper->value(chain) : 0;
}

Distribution const *StochasticNode::distribution() const
{
    return _dist;
}

double StochasticNode::logDensity(unsigned int chain) const
{
    if(!_dist->checkParameterValue(_parameters[chain], _dims))
	return JAGS_NEGINF;
    
    return _dist->logLikelihood(_data + _length * chain, _length,
				_parameters[chain], _dims,
				lowerLimit(chain), upperLimit(chain));
}

vector<double const *> const &StochasticNode::parameters(unsigned int chain) 
  const
{
    return _parameters[chain];
}

void StochasticNode::deterministicSample(unsigned int chain)
{
    _dist->typicalValue(_data + _length * chain, _length,
			_parameters[chain], _dims,
			lowerLimit(chain), upperLimit(chain));
}

void StochasticNode::randomSample(RNG *rng, unsigned int chain)
{
    _dist->randomSample(_data + _length * chain, _length, 
			_parameters[chain], _dims, 
			lowerLimit(chain), upperLimit(chain), rng);
}  

bool StochasticNode::isRandomVariable() const
{
    /* A Distribution with zero degrees of freedom represents an
       "observable function".  If a stochastic node such a
       distribution is observed, then we want it to generate a
       likelihood, so we say that it represents a random variable.
       However, if the node is unobserved, we want to treat it just
       like a deterministic node, so we say that it does not represent
       a random variable
    */

    return (_dist->df(_dims) == 0) ? isObserved() : true;
}

vector<vector<unsigned int> > const &StochasticNode::parameterDims() const
{
   return _dims;
}

bool StochasticNode::checkParentValues(unsigned int chain) const
{
    return _dist->checkParameterValue(_parameters[chain], _dims);
}

unsigned int df(StochasticNode const *snode)
{
    return snode->distribution()->df(snode->parameterDims());
}

void support(double *lower, double *upper, unsigned int length,
	     StochasticNode const *node, unsigned int chain)
{
    if (length != node->length()) {
	throw logic_error("Length mismatch in StochasticNode support");
    }

    node->distribution()->support(lower, upper, length,
				  node->parameters(chain),
				  node->parameterDims());
    if (isBounded(node)) {
	if (!node->distribution()->canBound()) {
	    throw logic_error("Bounded node has non-boundable distribution");
	}
	if (node->lowerBound()) {
	    double const *lb = node->lowerBound()->value(chain);
	    for (unsigned int i = 0; i < length; ++i) {
                if (lower[i] < lb[i])
                    lower[i] = lb[i];
	    }
	}
	if (node->upperBound()) {
	    double const *ub = node->upperBound()->value(chain);
	    for (unsigned int i = 0; i < length; ++i) {
                if (upper[i] > ub[i])
                    upper[i] = ub[i];
	    }
	}
    }
}

bool isSupportFixed(StochasticNode const *node)
{
    if (isBounded(node)) {
	if (!node->distribution()->canBound()) {
	    throw logic_error("Bounded node has non-boundable distribution");
	}
	if (node->lowerBound() && !node->lowerBound()->isObserved())
	    return false;
	if (node->upperBound() && !node->upperBound()->isObserved())
	    return false;
    }
    
    vector<Node const *> const &parents = node->parents();
    vector<bool> fixmask(parents.size());
    for (unsigned int i = 0; i < parents.size(); ++i) {
	fixmask[i] = parents[i]->isObserved();
    }
    return node->distribution()->isSupportFixed(fixmask);
}

bool isBounded(StochasticNode const *node)
{
    return node->lowerBound() || node->upperBound();
}

string StochasticNode::deparse(vector<string> const &parnames) const
{
    string name = _dist->name();
    name.append("(");
    unsigned int i = 0; 
    for ( ; i < _dist->npar(); ++i) {
	if (i != 0) {
	    name.append(",");
	}
	name.append(parnames[i]);
    }
    name.append(")");
    if (_lower || _upper) {
        name.append(" T(");
        if (_lower) {
            name.append(parnames[i++]);
        }
        name.append(",");
        if (_upper) {
            name.append(parnames[i++]);
        }
        name.append(")");
    }

    return name;
}

Node * StochasticNode::clone(vector<Node const *> const &parents) const
{
    vector<Node const *> param(parents);

    //Peel off upper and lower bounds
    Node const *upper = 0;
    if (_upper) {
	upper = param.back();
	param.pop_back();
    }
    Node const *lower = 0;
    if (_lower) {
	lower = param.back();
	param.pop_back();
    }
    
    return new StochasticNode(_dist, param, lower, upper);

}

bool StochasticNode::isDiscreteValued() const
{
    return _dist->isDiscreteValued();
}

bool StochasticNode::isObserved() const
{
    return _observed;
}

void StochasticNode::setObserved() 
{
    _observed = true;
} 

StochasticNode const *asStochastic(Node const* node) {
   return dynamic_cast<StochasticNode const*>(node);
}

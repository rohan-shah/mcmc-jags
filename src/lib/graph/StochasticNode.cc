#include <config.h>
#include <graph/Graph.h>
#include <graph/StochasticNode.h>
#include <graph/NodeError.h>
#include <distribution/Distribution.h>
#include <sarray/nainf.h>

#include <vector>
#include <stdexcept>
#include <string>
#include <stdexcept>

using std::vector;
using std::string;
using std::runtime_error;

static vector<unsigned int> mkDim(Distribution const *dist, 
				  vector<Node const *> const &parents)
{
  /* 
     Calculates dimension of stochastic node as a function of its
     parents
  */

  vector<vector<unsigned int> > parameter_dims(parents.size());
  for (unsigned long j = 0; j < parents.size(); ++j) {
    parameter_dims[j] = parents[j]->dim();
  }
  if (parameter_dims.size() != dist->npar()) {
    //FIXME: logic_error or runtime_error?
    throw runtime_error(string("Incorrect number of parameters for ") +
			"distribution " + dist->name());
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

StochasticNode::StochasticNode(Distribution const *dist, 
			       vector<Node const *> const &parameters,
			       Node const *lower, Node const *upper)
  : Node(mkDim(dist, parameters), mkParents(parameters, lower, upper)), 
    _dist(dist), _parameters(nchain()), _lower(lower), _upper(upper)
{
 
  if (parameters.size() != _dist->npar()) {
    throw NodeError(this, "Incorrect number of parameters for distribution");
  }
  
  _dims.reserve(parameters.size());
  for (unsigned int i = 0; i < parameters.size(); ++i) {
    _dims.push_back(parameters[i]->dim());
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
    _parameters[n].reserve(parameters.size() + 2 * (_dist->canBound()));
    
    for (unsigned int i = 0; i < parameters.size(); ++i) {
      _parameters[n].push_back(parameters[i]->value(n));
    }
    _parameters[n].push_back(lower ? lower->value(n) : 0);
    _parameters[n].push_back(upper ? upper->value(n) : 0);
  }
  
  if (dist->isDiscreteValued()) {
    setDiscreteValued();
  }
}

StochasticNode::~StochasticNode()
{
}

bool StochasticNode::isBounded() const
{
  return _lower || _upper;
}

/*
SArray const *StochasticNode::lowerBound(unsigned int chain)
{
  return _dist->lowerBound(_parameters[chain]);
}

SArray const *StochasticNode::upperBound(unsigned int chain)
{
  return _dist->upperBound(_parameters[chain]);
}
*/

Node const *StochasticNode::lowerBound()
{
  return _lower;
}

Node const *StochasticNode::upperBound()
{
  return _upper;
}

/*
void StochasticNode::setBounds(Node *lBound, Node *uBound)
{
  if (!_dist->canBound()) {
    throw NodeError(this, string("Distribution ") + _dist->name()
                    + " cannot be bounded");
  }
  if (_parameters.empty()) {
    throw NodeError(this, "Cannot set bounds before parameters");
  }
  if ((lBound && lBound->dim(true) != dim(true)) || 
      (uBound && uBound->dim(true) != dim(true)))
    {
      throw NodeError(this,"Dimension mismatch when setting bounds");
    }

  if (lBound) {
    for (unsigned int n = 0; n < nchain(); ++n) {
       SArray const *lb = _parameters[n][_parameters.size() - 2];
       if (lb != 0) {
         throw NodeError(this, "Attempt to reset lower bound");
       }
       _parameters[n][_parameters.size() - 2] = lBound->data(n);
    }
    addParent(lBound);      
  }
  if (uBound) {
    for (unsigned int n = 0; n < nchain(); ++n) {
       SArray const *ub = _parameters[n][_parameters.size() - 1];
       if (ub != 0) {
         throw NodeError(this, "Attempt to reset upper bound");
       }
       _parameters[n][_parameters.size() - 1] = uBound->data(n);
    }
    addParent(uBound);
  }
}
*/

Distribution const *StochasticNode::distribution() const
{
    return _dist;
}

/*
void StochasticNode::setParameters(vector<Node *> const &parameters)
{
  if (_parameters.size() != 0) {
    throw NodeError(this, "Attempt to reset parameters");
  }

  for (unsigned int n = 0; n < nchain(); ++n) {
    _parameters[n].reserve(parameters.size() + 2 * (_dist->canBound()));
  }

  for (unsigned int i = 0; i < parameters.size(); ++i) {
     for (unsigned int n = 0; n < nchain(); ++n) {
        _parameters[n].push_back(parameters[i]->data(n));
     }
     this->addParent(parameters[i]);
  }

  // Push back zeros for the upper and lower bounds
  if (_dist->canBound()) {
    for (unsigned int n = 0; n < nchain(); ++n) {
       _parameters[n].push_back(0);
       _parameters[n].push_back(0);
    }
  }
  
  if (_parameters[0].size() != _dist->npar()) {
    throw NodeError(this, "Incorrect number of parameters for distribution");
  }
  if (!_dist->checkParameterDim(_parameters[0])) {
    throw NodeError(this,"Invalid parameter dimensions for distribution");
  }
  if (_dist->dim(_parameters[0]) != dim(true)) {
    throw NodeError(this, "Dimension mismatch between parameters and Node");
  }
}
*/

double StochasticNode::logDensity(unsigned int chain) const
{
    if(!_dist->checkParameterValue(_parameters[chain], _dims))
	return JAGS_NEGINF;
    
    return _dist->logLikelihood(_data + _length * chain, _length,
				_parameters[chain], _dims);
}

vector<double const *> const &StochasticNode::parameters(unsigned int chain) 
  const
{
  return _parameters[chain];
}

void StochasticNode::deterministicSample(unsigned int chain)
{
    _dist->typicalValue(_data + _length * chain, _length,
			_parameters[chain], _dims);
}

void StochasticNode::randomSample(RNG *rng, unsigned int chain)
{
    _dist->randomSample(_data + _length * chain, _length, 
			_parameters[chain], _dims, rng);
}  

StochasticNode const *asStochastic(Node const *node)
{
  return dynamic_cast<StochasticNode const*>(node);
}

bool StochasticNode::isVariable() const
{
  return true;
  /*
  if (_parameters.empty())
    throw logic_error("Cannot determine if node with no parameters is stochastic");

  if (_dist->isDeterministic() == 0) {
    //Deterministic distributions are considered stochastic only
    //if they are observed, in which case they generate a likelihood.
     return isObserved(this);
  }
  else {
     return true;
  }
  */
}

vector<vector<unsigned int> > const &StochasticNode::parameterDims() const
{
   return _dims;
}

bool StochasticNode::checkParentValues(unsigned int chain) const
{
    return _dist->checkParameterValue(_parameters[chain], _dims);
}

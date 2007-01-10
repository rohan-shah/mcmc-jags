#include <config.h>
#include <distribution/Distribution.h>
#include <graph/StochasticNode.h>
#include <graph/Graph.h>
#include "DSumSampler.h"

#include <cfloat>
#include <climits>
#include <stdexcept>
#include <set>
#include <algorithm>
#include <cmath>

#include <JRmath.h>

using std::set;
using std::vector;
using std::invalid_argument;
using std::max;
using std::min;
using std::exp;

DSumSampler::DSumSampler(vector<StochasticNode *> const &nodes,
			 Graph const &graph, unsigned int chain)
    : Slicer(nodes, graph, 2, 10), _chain(chain)
{
    if (!canSample(nodes, graph)) {
	throw invalid_argument("Can't construct DSumSampler");
    }

    Node const *dsum = 0;
    set<Node*> const *children = nodes[0]->children();
    for (set<Node*>::const_iterator p = children->begin(); p != children->end();	 ++p) 
	{
	  if (asStochastic(*p) && (*p)->isObserved() &&
	      asStochastic(*p)->distribution()->name() == "dsum")
	    {
	      dsum = *p;
	      break;
	    }
	}
  
    _sum = static_cast<long>(*dsum->value(chain));
    _x = *nodes[0]->value(chain);
    //Make sure values are consistent at start
    double x2 = _sum - static_cast<long>(_x);
    nodes[1]->setValue(&x2,1,chain);
}

DSumSampler::~DSumSampler()
{
}

bool DSumSampler::canSample(vector<StochasticNode *> const &nodes,
			    Graph const &graph)
{
    if (nodes.size() != 2)
	return false;

    if (!(graph.contains(nodes[0]) && graph.contains(nodes[1])))
	return false;
    
    for (unsigned int i = 0; i < 2; ++i) {
	// Nodes must be scalar ...
	if (nodes[i]->length() != 1)
	    return false;
    
	// discrete-valued ...
	if (!nodes[i]->isDiscreteValued())
	    return false;
    }

    /* Nodes must be direct parents of a single observed stochastic
       node with distribution DSum  */
    vector<StochasticNode const*> stoch_nodes;
    vector<DeterministicNode*> dtrm_nodes;
    classifyChildren(nodes, graph, stoch_nodes, dtrm_nodes);
    if (!dtrm_nodes.empty())
	return false;
    if (stoch_nodes.size() != 1)
	return false;
    if (!stoch_nodes[0]->isObserved())
	return false;
    if (stoch_nodes[0]->distribution()->name() != "dsum")
	return false;

    // And so, their work was done...
    return true;
}

/*
void DSumSampler::update()
{
  // Calculate lower and upper limits of first variable
  DistDiscrete const *idist0 = asDiscrete(nodes()[0]->distribution());
  DistDiscrete const *idist1 = asDiscrete(nodes()[1]->distribution());
  long ll = max(idist0->lowerLimit(nodes()[0]->parameters()),
		_sum - idist1->upperLimit(nodes()[1]->parameters()));
  long ul = min(idist0->upperLimit(nodes()[0]->parameters()),
		_sum - idist1->lowerLimit(nodes()[1]->parameters()));

  long N = ul - ll + 1;
  double *p = new double[N];
  
  //Setup the vectors used by Sampler::setValue
  double v[2];
  vector<double const*> value;
  value.push_back(&v[0]);
  value.push_back(&v[1]);
  vector<long> length = vector<long>(2,1);

  //Calculate likelihood for each possible value
  for (int i = 0; i < N; ++i) {
    v[0] = ll + i;
    v[1] = _sum - ll - i;
    setValue(value, length);
    p[i] = exp(logFullConditional());
  }

  //Normalize
  double sump = 0;
  for (int i = 0; i < N; ++i) {
    sump += p[i];
  }

  //Sample
  double urand = runif(0.0, sump);
  long i;
  sump = 0;
  for (i = 0; i < N - 1; ++i) {
    sump += p[i];
    if (sump > urand) {
      break;
    }
  }

  //Set sampled value
  v[0] = ll + i;
  v[1] = _sum - ll - i;
  setValue(value, length);

  delete [] p;
}
*/

void DSumSampler::setValue(double x)
{
  _x = x;
  double value[2];
  value[0] = static_cast<long>(x);
  value[1] = _sum - value[0];
  Sampler::setValue(value, 2, _chain);
}

double DSumSampler::value() const
{
  return _x;
}

void DSumSampler::getLimits(double *lower, double *upper) const
{
  vector<StochasticNode *> const &n = nodes();
  double l0, u0, l1, u1;
  n[0]->distribution()->support(&l0, &u0, 1U, n[0]->parameters(_chain),
                                n[0]->parameterDims());
  n[1]->distribution()->support(&l1, &u1, 1U, n[1]->parameters(_chain),
                                n[1]->parameterDims());
  *lower = max(l0, _sum - u1);
  *upper = min(u0, _sum - l1);
}

void DSumSampler::update(RNG *rng)
{
  updateDouble(rng);
}

double DSumSampler::logFullConditional() const
{
  return Sampler::logFullConditional(_chain);
}

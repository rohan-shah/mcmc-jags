#include <config.h>
#include <rng/RNG.h>
#include <sampler/SingletonGraphView.h>
//#include <graph/Graph.h>
#include <graph/StochasticNode.h>
//#include <distribution/Distribution.h>
//#include <module/ModuleError.h>
#include <sampler/Linear.h>
#include <util/nainf.h>

#include "SumMethod.h"
#include <cmath>
//#include <set>
#include <numeric>
#include <cfloat>

using std::vector;
//using std::log;
//using std::exp;
//using std::fabs;
//using std::set;
using std::accumulate;
using std::min;
using std::max;
using std::ceil;

namespace jags {
    
    namespace bugs {

	static double sumValue(vector<Node const*> const &x, unsigned int ch)
	{
	    double y = 0;
	    for (vector<Node const*>::const_iterator p = x.begin();
		 p != x.end(); ++p)
	    {
		double const *v = (*p)->value(ch);
		unsigned int n = (*p)->length();
		y = accumulate(v, v+n, y);
	    }
	    return y;
	}
	    
	StochasticNode const *
	SumMethod::isCandidate(StochasticNode *snode, Graph const &graph)
	{
	    //We can only sample scalar nodes
	    if (snode->length() != 1) return 0;
	    
	    SingletonGraphView gv(snode, graph);

	    /* Check stochastic children
	       
	       There must be a single stochastic child, which is
	       observed and has the distribution "sum".  
	    */
	    vector<StochasticNode*> const &schildren = gv.stochasticChildren();
	    if (schildren.size() != 1) return 0;
	    StochasticNode const *schild = schildren.front();
	    if (schild->distribution()->name() != "sum") return 0;
	    if (!isObserved(schild)) return 0;
	    
	    /* Deterministic descendants must be an additive function
	     * of snode
	     */
	    if (!checkAdditive(&gv, false)) return 0;

	    return schild;
	}
	
	bool SumMethod::canSample(vector<StochasticNode*> const &snodes,
				  Graph const &graph)
	{
	    unsigned int len = snodes.size();
	    if (len < 2) return false;

	    Node const *sumchild = isCandidate(snodes[0], graph);
	    if (sumchild == 0) return false;
	    for (unsigned int i = 1; i < snodes.size(); ++i) {
		if (isCandidate(snodes[i], graph) != sumchild) return false;
	    }

	    //Is the whole thing additive with a fixed intercept?
	    GraphView gv(snodes, graph);
	    if (!checkAdditive(&gv, graph, true)) return false;

	    //Now check intercept
	    
	    //Store old value
	    const unsigned int ch = 0;
	    vector<double> xold(len);
	    gv.getValue(xold, ch);

	    //Create new value
	    StochasticNode const *sumnode = gv.stochasticChildren().front();
	    gv.setValue(vector<double>(len, 0), ch);
	    bool ok = (sumValue(sumnode->parents(), ch) != 0);
	    gv.setValue(xold, ch);
	    if (!ok) return false;

	    //Check consistency of discreteness
	    if (snodes.empty()) return false;
	    bool discrete = sumnode->isDiscreteValued();
	    for (unsigned int i = 0; i < snodes.size(); ++i) {
		if (snodes[i]->isDiscreteValued() != discrete) return false;
		if (snodes[i]->length() != 1) return false;
	    }
	    return true;
	}

	SumMethod::SumMethod(GraphView const *gv, unsigned int chain)
	    : _gv(gv), _chain(chain),
	      _sum(gv->stochasticChildren()[0]->value(chain)[0]),
	      _discrete(gv->stochasticChildren()[0]->isDiscreteValued()),
	      _x(gv->length()), _i(0), _j(0), _sumdiff(0), _iter(0),
	      _width(2), _max(10), _adapt(true)
	{
	    gv->getValue(_x, chain);
	}
	
	SumMethod::~SumMethod()
	{
	}

	void SumMethod::updateStep(RNG *rng)
	{
	    // Test current value
	    double g0 = logDensity();
	    if (!jags_finite(g0)) {
		return;
	    }

	    // Generate auxiliary variable
	    double z = g0 - rng->exponential();

	    //Discrete width
	    double width = _discrete ? ceil(_width) : _width;
	    
	    // Generate random interval of width "_width" about current value
	    double left = _discrete ? floor(rng->uniform() * (width + 1)) :
		rng->uniform() * width;
	    double xold = value();
	    double L = xold - left;
	    double R = L + width;

	    double lower = JAGS_NEGINF, upper = JAGS_POSINF;
	    getLimits(&lower, &upper);

	    // Stepping out 

	    // Randomly set number of steps in left and right directions,
	    // subject to the limit in the maximal size of the interval
	    int j = static_cast<int>(rng->uniform() * _max);
	    int k = _max - 1 - j;

	    if (L < lower) {
		L = lower;
	    }
	    else {
		setValue(L);
		while (j-- > 0 && logDensity() > z) {
		    L -= width;
		    if (L < lower) {
			L = lower;
			break;
		    }
		    setValue(L);
		}
	    }

	    if (R > upper) {
		R = upper;
	    }
	    else {
		setValue(R);
		while (k-- > 0 && logDensity() > z) {
		    R += width;
		    if (R > upper) {
			R = upper;
			break;
		    }
		    setValue(R);
		}
	    }

	    // Keep sampling from the interval until acceptance (the loop is
	    // guaranteed to terminate).
	    double xnew;
	    for(;;) {
		if (_discrete) {
		    xnew =  L + floor(rng->uniform() * (R - L + 1));
		}
		else {
		    xnew =  L + rng->uniform() * (R - L);
		}
		setValue(xnew);
		double g = logDensity();
		if (g >= z - DBL_EPSILON) {
		    // Accept point
		    break;
		}
		else {
		    // shrink the interval
		    if (xnew < xold) {
			L = xnew;
		    }
		    else {
			R = xnew;
		    }
		}
	    }
	    
	    if (_adapt) {
		_sumdiff += fabs(xnew - xold);
	    }
	}

	void SumMethod::update(RNG *rng)
	{
	    unsigned int len = _gv->length();
	    for(_i = 0; _i < len; ++_i) {
		_j = static_cast<unsigned int>(rng->uniform() * (len-1));
		if (_j >= _i) _j++;
		updateStep(rng);
	    }
	    if (_adapt) {
		if (++_iter % 50 == 0) {
		    _width = _sumdiff / (50 * len);
		    _sumdiff = 0;
		}
	    }
	    _gv->setValue(_x, _chain);
	}

	void SumMethod::setValue(double x)
	{
	    double delta = x - _x[_i];
	    _x[_i] = x;
	    _x[_j] -= delta;
	    
	    _gv->nodes()[_i]->setValue(&_x[_i], 1, _chain);
	    _gv->nodes()[_j]->setValue(&_x[_j], 1, _chain);
	}

	double SumMethod::value() const
	{
	    return _x[_i];
	}

	void SumMethod::getLimits(double *lower, double *upper) const
	{
	    vector<StochasticNode *> const &n = _gv->nodes();
	    double li, ui, lj, uj;
	    n[_i]->support(&li, &ui, 1U, _chain);
	    n[_j]->support(&lj, &uj, 1U, _chain);
	    double sum_ij = n[_i]->value(_chain)[0] + n[_j]->value(_chain)[0];
	    *lower = max(li, sum_ij - uj);
	    *upper = min(ui, sum_ij - lj);
	}

	void SumMethod::adaptOff()
	{
	    _adapt=false;
	}

	bool SumMethod::isAdaptive()
	{
	    boo
	}
	
    } // namespace bugs
} //namespace jags


#include <config.h>

#include "AlbertChib.h"
#include "KS.h"

#include <graph/StochasticNode.h>
#include <graph/LinkNode.h>
#include <sampler/GraphView.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>
#include <cmath>
#include <module/ModuleError.h>

using std::vector;
using std::string;
using std::log;
using std::exp;
using std::fabs;

//Regularization penalty to stop precision going to zero in logistic model
#define REG_PENALTY 0.001

//FIXME: maybe use R math library here

//Random sample from a left-truncated logistic distribution
static double llogit(double left, RNG *rng, double mu)
{
    double qleft = 1/(1 + exp(mu-left));
    double x = qleft + (1 - qleft) * rng->uniform();
    return mu + log(x) - log(1 - x);
}

//Random sample from a right-truncated logistc distribution
static double rlogit(double right, RNG *rng, double mu)
{
    double qright = 1/(1 + exp(mu-right));
    double x = qright * rng->uniform();
    return mu + log(x) - log(1 - x);
}

#define CHILD(i) (_view->stochasticChildren()[i])

namespace glm {

    AlbertChib::AlbertChib(GraphView const *view,
			   vector<GraphView const *> const &sub_views,
			   unsigned int chain, bool gibbs)
	: BinaryGLM(view, sub_views, chain), _gibbs(gibbs), _aux_init(true)
    {
    }

    void AlbertChib::update(RNG *rng)
    {
	if (_aux_init) {
	    initAuxiliary(rng);
	    _aux_init = false;
	}

	/*
	  Note that we must update the auxiliary variables *before*
	  calling updateLM. This ordering is important for models with
	  a variable design matrix (e.g. measurement error models).
	*/

	unsigned int nrow = _view->stochasticChildren().size();
	
	double y, mu;
	for (unsigned int r = 0; r < nrow; ++r) {
	    switch(_outcome[r]) {
	    case BGLM_NORMAL:
		break;
	    case BGLM_PROBIT:
		y = CHILD(r)->value(_chain)[0];
		if (y == 1) {
		    _z[r] = lnormal(0, rng, getMean(r));
		}
		else if (y == 0) {
		    _z[r] = rnormal(0, rng, getMean(r));
		}
		else {
		    throwLogicError("Invalid child value in HolmesHeld");
		}
		break;
	    case BGLM_LOGIT:
		y = CHILD(r)->value(_chain)[0];
		mu = getMean(r);
		if (y == 1) {
		    _z[r] = llogit(0, rng, mu);
		}
		else if (y == 0) {
		    _z[r] = rlogit(0, rng, mu);
		}
		else {
		    throwLogicError("Invalid child value in HolmesHeld");
		}
		_tau[r] = REG_PENALTY + 1/sample_lambda(fabs(_z[r] - mu), rng);
		break;
	    }
	}

	if (_gibbs) {
	    updateLMGibbs(rng);
	}
	else {
	    updateLM(rng);
	}
    }
	
    string AlbertChib::name() const
    {
	if (_gibbs)
	    return "Albert-Chib-Gibbs";
	else
	    return "Albert-Chib";
    }
}
    

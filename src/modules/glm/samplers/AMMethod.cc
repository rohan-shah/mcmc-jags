#include <config.h>

#include "AMMethod.h"
#include "AuxMixBinomial.h"
#include "AuxMixPoisson.h"
#include "AuxMixNormal.h"
#include "AuxMix.h"

#include <graph/LinkNode.h>
#include <graph/StochasticNode.h>
#include <module/ModuleError.h>

using std::vector;
using std::string;

static double const & one() 
{
    static const double x = 1;
    return x;
}

namespace glm {

    AMMethod::AMMethod(GraphView const *view,
		       vector<GraphView const *> const &sub_views,
		       unsigned int chain)
	: GLMMethod(view, sub_views, chain, true), 
	  _aux(view->stochasticChildren().size())
    {
	vector<StochasticNode const*> const &children = 
	    _view->stochasticChildren();

	for (unsigned int i = 0; i < children.size(); ++i) {

	    StochasticNode const *y = children[i];
	    Node const *eta = y->parents()[0]->parents()[0];
	    Node const *tau = 0;

	    switch(GLMMethod::getFamily(y)) {
	    case GLM_BERNOULLI:
		_aux[i] = new AuxMixBinomial(eta->value(chain)[0], one(),
					     y->value(chain)[0]);
		break;
	    case GLM_BINOMIAL: 
		tau = y->parents()[1];
		_aux[i] = new AuxMixBinomial(eta->value(chain)[0],
					     tau->value(chain)[0],
					     y->value(chain)[0]);
		break;
	    case GLM_POISSON:
		_aux[i] = new AuxMixPoisson(eta->value(chain)[0],
					    y->value(chain)[0]);
		break;
	    case GLM_NORMAL:
		tau = y->parents()[1];
		_aux[i] = new AuxMixNormal(tau->value(chain)[0],
					   y->value(chain)[0]);
		break;
	    default:
		throwLogicError("Invalid family in AMMethod");
		break;
	    }
	}
    }

    AMMethod::~AMMethod()
    {
	for (unsigned int i = 0; i < _aux.size(); ++i) {
	    delete _aux[i];
	}
    }

    double AMMethod::getValue(unsigned int i) const
    {
	return _aux[i]->value();
    }

    double AMMethod::getPrecision(unsigned int i) const
    {
	return _aux[i]->precision();
    }

    void AMMethod::update(RNG *rng)
    {
	for (unsigned int r = 0; r < _aux.size(); ++r)
	{
	    _aux[r]->update(rng);
	}
	updateLM(rng);
    }

    string AMMethod::name() const
    {
	return "AuxiliaryMixture";
    }
}
    

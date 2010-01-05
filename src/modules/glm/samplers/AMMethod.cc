#include <config.h>

#include "AMMethod.h"
#include "AuxMixBinomial.h"
#include "AuxMixPoisson.h"
#include "AuxMixNormal.h"
#include "AuxMix.h"

#include <graph/LinkNode.h>
#include <graph/StochasticNode.h>
#include <function/InverseLinkFunc.h>

#include <stdexcept>

using std::vector;
using std::string;
using std::logic_error;

static double const & one() 
{
    static const double x = 1;
    return x;
}

static bool checkOutcome(StochasticNode const *snode)
{
    LinkNode const *lnode = dynamic_cast<LinkNode const*>(snode->parents()[0]);
    string ln;
    if (lnode) {
	ln = lnode->link()->linkName();
    }


    switch(glm::GLMMethod::getFamily(snode)) {
    case GLM_BINOMIAL: case GLM_BERNOULLI:
	if (ln != "logit") {
	    return false;
	}
	break;
    case GLM_POISSON:
	if (ln != "log") {
	    return false;
	}
	break;
    case GLM_NORMAL:
	if (lnode) {
	    return false;
	}
	break;
    default:
	return false;
    }

    return true;
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

	    if (!checkOutcome(children[i])) 
		throw logic_error("Invalid outcome in AMMethod");

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
		throw logic_error("Invalid family in AMMethod");
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
    

#include <config.h>

#include "Logit.h"
#include "KS.h"

#include <graph/StochasticNode.h>
#include <sampler/Updater.h>
#include <rng/TruncatedNormal.h>
#include <rng/RNG.h>

#include <stdexcept>
#include <cmath>

using std::string;
using std::vector;

namespace glm {

    Logit::Logit(Updater const *updater,
		 vector<Updater const *> const &sub_updaters,
		 unsigned int chain)
	: ScaleMixture(updater, sub_updaters, chain), 
	  _tau(updater->stochasticChildren().size(), 1)
    {
    }

    double Logit::getPrecision(unsigned int i) const 
    {
	return _tau[i];
    }
    
    string Logit::name() const
    {
	return "Logit";
    }
    
    void Logit::update(RNG *rng)
    {
	updateLM(rng);

	for (unsigned int r = 0; r < _tau.size(); ++r)
	{
	    double delta = fabs(getValue(r) - getMean(r));
	    _tau[r] = 1/sample_lambda(delta, rng);
	}
    }
}

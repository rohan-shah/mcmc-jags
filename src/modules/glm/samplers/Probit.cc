#include <config.h>

#include "Probit.h"
#include <rng/RNG.h>

using std::vector;
using std::string;

namespace glm {

    Probit::Probit(Updater const *updater,
		   vector<Updater const *> const &sub_updaters,
		   unsigned int chain)
	: ScaleMixture(updater, sub_updaters, chain)
    {
    }

    double 
    Probit::getPrecision(unsigned int i) const 
    {
	return 1;
    }

    void Probit::updatePrecision(RNG *rng) 
    {
    }

    string Probit::name() const
    {
	return "Probit";
    }

}

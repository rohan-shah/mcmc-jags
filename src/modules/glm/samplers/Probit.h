#ifndef PROBIT_H_
#define PROBIT_H_

#include "ScaleMixture.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class Probit : public ScaleMixture {
    public:
	Probit(Updater const *updater, 
	       std::vector<Updater const *> const &sub_updaters,
	       unsigned int chain);
	double getPrecision(unsigned int i) const;
	void updatePrecision(RNG *rng);
	std::string name() const;
    };
    
}

#endif /* PROBIT_H_ */

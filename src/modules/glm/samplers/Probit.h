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
	std::string name() const;
	void update(RNG *rng);
    };
    
}

#endif /* PROBIT_H_ */

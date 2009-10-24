#ifndef LOGIT_H_
#define LOGIT_H_

#include "ScaleMixture.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class Logit : public ScaleMixture {
	std::vector<double> _z;
	std::vector<double> _tau;
    public:
	Logit(Updater const *updater, 
	       std::vector<Updater const *> const &sub_updaters,
	       unsigned int chain);
	double getPrecision(unsigned int i) const;
	std::string name() const;
	void update(RNG *rng);
    };
    
}

#endif /* LOGIT_H_ */

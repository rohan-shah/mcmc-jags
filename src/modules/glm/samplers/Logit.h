#ifndef LOGIT_H_
#define LOGIT_H_

#include "GLMMethod.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class Probit : public GLMMethod {
	std::vector<double> _z;
	std::vector<double> _tau;
    public:
	Probit(Updater const *updater, 
	       std::vector<Updater const *> const &sub_updaters,
	       unsigned int chain);
	double getMean(unsigned int i) const;
	double getPrecision(unsigned int i) const;
	double getValue(unsigned int i) const;
	void initAuxiliary(RNG *rng);
	void updateAuxiliary(double *b, csn const *N, RNG *rng);
	std::string name() const;
    };
    
}

#endif /* LOGIT_H_ */

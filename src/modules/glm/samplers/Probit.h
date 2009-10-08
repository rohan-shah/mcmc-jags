#ifndef PROBIT_H_
#define PROBIT_H_

#include "GLMMethod.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class Probit : public GLMMethod {
	std::vector<double> _z;
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

#endif /* PROBIT_H_ */

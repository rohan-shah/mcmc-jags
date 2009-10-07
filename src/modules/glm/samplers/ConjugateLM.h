#ifndef CONJUGATE_LM_H_
#define CONJUGATE_LM_H_

#include "GLMMethod.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class ConjugateLM : public GLMMethod {
    public:
	ConjugateLM(Updater const *updater, unsigned int chain);
	double getMean(unsigned int i) const;
	double getPrecision(unsigned int i) const;
	double getValue(unsigned int i) const;
	void initAuxiliary(RNG *rng);
	void updateAuxiliary(double *b, csn const *N, RNG *rng);
	std::string name() const;
    };
    
}

#endif /* CONJUGATE_LM_H_ */

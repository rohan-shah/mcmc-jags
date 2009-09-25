#ifndef CONJUGATE_LM_H_
#define CONJUGATE_LM_H_

#include "GLMMethod.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class ConjugateLM : public GLMMethod {
	Updater const *_updater;
    public:
	ConjugateLM(Updater const *updater);
	double getMean(unsigned int i, unsigned int chain) const;
	double getPrecision(unsigned int i, unsigned int chain) const;
	double getValue(unsigned int i, unsigned int chain) const;
	std::string name() const;
    };
    
}

#endif /* CONJUGATE_LM_H_ */

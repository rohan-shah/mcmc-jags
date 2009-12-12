#ifndef BINARY_GLM_H_
#define BINARY_GLM_H_

#include "GLMMethod.h"

enum BGLMOutcome {BGLM_NORMAL, BGLM_LOGIT, BGLM_PROBIT};

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class BinaryGLM : public GLMMethod {
    protected:
	std::vector<BGLMOutcome> _outcome;
	std::vector<double> _z;
	std::vector<double> _tau;
    public:
	BinaryGLM(Updater const *updater, 
		  std::vector<Updater const *> const &sub_updaters,
		  unsigned int chain);
	void initAuxiliary(RNG *rng);
	double getValue(unsigned int i) const;
	double getPrecision(unsigned int i) const;
    };
    
}

#endif /* BINARY_GLM_H__H_ */

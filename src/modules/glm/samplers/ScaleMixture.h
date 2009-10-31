#ifndef SCALE_MIXTURE_H_
#define SCALE_MIXTURE_H_

#include "GLMMethod.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class ScaleMixture : public GLMMethod {
	std::vector<double> _z;
    public:
	ScaleMixture(Updater const *updater, 
		     std::vector<Updater const *> const &sub_updaters,
		     unsigned int chain);
	double getMean(unsigned int i) const;
	double getValue(unsigned int i) const;
	void initAuxiliary(RNG *rng);
	void updateAuxiliary(double *b, csn *N, RNG *rng);
    };
    
}

#endif /* SCALE_MIXTURE_H_ */

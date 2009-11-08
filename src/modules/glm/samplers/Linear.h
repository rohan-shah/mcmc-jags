#ifndef LINEAR_H_
#define LINEAR_H_

#include "GLMMethod.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class Linear : public GLMMethod {
	bool _gibbs;
    public:
	Linear(Updater const *updater,
	       std::vector<Updater const *> const &sub_updaters,
	       unsigned int chain, bool gibbs);
	double getMean(unsigned int i) const;
	double getPrecision(unsigned int i) const;
	double getValue(unsigned int i) const;
	void initAuxiliary(RNG *rng);
	void updateAuxiliary(double *b, csn *N, RNG *rng);
	void update(RNG *rng);
	std::string name() const;
    };
    
}

#endif /* LINEAR_H_ */

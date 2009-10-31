#ifndef HOLMES_HELD_H_
#define HOLMES_HELD_H_

#include "GLMMethod.h"

enum HHOutcome {HH_NORMAL, HH_LOGIT, HH_PROBIT};

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class HolmesHeld : public GLMMethod {
	std::vector<HHOutcome> _outcome;
	std::vector<double> _z;
	std::vector<double> _tau;
    public:
	HolmesHeld(Updater const *updater, 
		   std::vector<Updater const *> const &sub_updaters,
		   unsigned int chain);
	double getMean(unsigned int i) const;
	double getValue(unsigned int i) const;
	double getPrecision(unsigned int i) const;
	void initAuxiliary(RNG *rng);
	void updateAuxiliary(double *b, csn *N, RNG *rng);
	std::string name() const;
	void update(RNG *rng);
    };
    
}

#endif /* HOLMES_HELD_H_ */

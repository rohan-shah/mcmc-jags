#ifndef IWLS_H_
#define IWLS_H_

extern "C" {
#include <cs.h>
}

#include "GLMMethod.h"

enum GLMFamily {GLM_BERNOULLI=0, GLM_BINOMIAL, GLM_POISSON, GLM_UNKNOWN};

class LinkNode;

namespace glm {

    /**
     * @short Iteratively Weighted Least Squares
     */
    class IWLS : public GLMMethod {
	std::vector<LinkNode const *> _link;
	std::vector<GLMFamily> _family;
	bool _init;
	double _w;
        double var(unsigned int i) const;
        double logPTransition(std::vector<double> const &xorig,
                              std::vector<double> const &x,
                              double const *b, cs const *A);
    public:
	IWLS(Updater const *updater, 
	     std::vector<Updater const *> const &sub_updaters,
	     unsigned int chain);
	std::string name() const;
	double getMean(unsigned int i) const;
	double getPrecision(unsigned int i) const;
	double getValue(unsigned int i) const;
	void initAuxiliary(RNG *rng);
	void updateAuxiliary(double *b, csn const *N, RNG *rng);
	void update(RNG *rng);
	static GLMFamily getFamily(StochasticNode const *snode);
    };

}
#endif /* GLM_METHOD_H_ */

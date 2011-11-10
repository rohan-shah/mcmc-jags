#ifndef CONJUGATE_F_METHOD_H_
#define CONJUGATE_F_METHOD_H_

#include <sampler/SampleMethod.h>
#include <sampler/GraphView.h>

namespace glm {

    /**
     * @short Sampling method for precision parameters of random effects.
     *
     * When an F distribution is used as a prior for the precision of
     * normally distributed random effects in a linear model, it has a
     * conditionally conjugate distribution using a redundant
     * parametrization.
     *
     * This is described by Gelman A (2006) Prior distributions for
     * variance parameters in hierarchical models. Bayesian Analysis
     * 1:515–533. Gelman describes the half-t distribution of the standard
     * deviation. This corresponds to an F(m,1) prior on the precision
     * parameter.
     *
     * This is currently an experimental sampling method.
     */
    class ConjugateFMethod {
	GraphView *_gv1, *_gv2;
	unsigned int _chain;
	double _scale, _tau;
	double *_coef;
    public:
	ConjugateFMethod(GraphView *gv1, GraphView *gv2, unsigned int chain);
	~ConjugateFMethod();
	void update(RNG *rng);
    };

}

#endif /* CONJUGATE_F_METHOD_H_ */

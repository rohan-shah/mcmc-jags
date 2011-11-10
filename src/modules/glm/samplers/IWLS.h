#ifndef IWLS_H_
#define IWLS_H_

#include "GLMMethod.h"

class LinkNode;

namespace glm {

    /**
     * @short Sampling method for GLMs with Iteratively Weighted Least Squares
     *
     * A stochastic version of the Iteratively Weighted Least Squares
     * algorithm (IWLS) was proposed by Gammerman D (1997). Efficient
     * sampling from the posterior distribution in generalized linear
     * mixed models. Statistics and Computing 7:57-68.  The algorithm
     * is a Metropolis-Hastings algorithm that uses IWLS to generate a
     * proposal.
     *
     * The IWLS method is restricted to fixed effects as it relies
     * on an asymptotic approximation that does not hold for random
     * effects models.
     */
    class IWLS : public GLMMethod {
	std::vector<LinkNode const *> _link;
	std::vector<GLMFamily> _family;
	bool _init;
	double _w;
        double var(unsigned int i) const;
        double logPTransition(std::vector<double> const &xorig,
                              std::vector<double> const &x,
                              double *b, cholmod_sparse *A);
    public:
	IWLS(GraphView const *view, 
	     std::vector<GraphView const *> const &sub_views,
	     unsigned int chain);
	/**
	 * Returns "IWLS"
	 */
	std::string name() const;
	/**
	 * Returns the value of the outcome variable in a local linear
	 * approximation to the GLM
	 */
	double getValue(unsigned int i) const;	
	/**
	 * Returns the precision of the outcome variable in a local
	 * linear approximation to the GLM
	 */
	double getPrecision(unsigned int i) const;
	/**
	 * Generates a new proposal from an approximation to the posterior
	 * distribution derived by a single IWLS step, then carries out
	 * a Metropolis-Hastings acceptance step.
	 */
	void update(RNG *rng);
    };

}

#endif /* IWLS_H_ */

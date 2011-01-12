#ifndef SCALAR_STOCHASTIC_NODE_H_
#define SCALAR_STOCHASTIC_NODE_H_

#include <graph/StochasticNode.h>

class ScalarDist;

/**
 * @short Scalar Node defined by the BUGS-language operator ~
 */
class ScalarStochasticNode : public StochasticNode {
    ScalarDist const * const _dist;
    void sp(double *lower, double *upper, unsigned int length,
	    unsigned int chain) const;
public:
    /**
     * Constructs a new StochasticNode given a distribution, a vector
     * of parent nodes, considered as parameters to the distribution,
     * and, optionally, upper and lower bounds. If bounds are given
     * then the distribution of the constructed StochasticNode is
     * truncated at the value of the bounds. 
     */
    ScalarStochasticNode(ScalarDist const *dist,
			 std::vector<Node const *> const &parameters,
			 Node const *lower, Node const *upper);
    double logDensity(unsigned int chain, PDFType type) const;
    void randomSample(RNG *rng, unsigned int chain);
    void truncatedSample(RNG *rng, unsigned int chain,
			 double const *lower, double const *upper);
    void deterministicSample(unsigned int chain);
    bool checkParentValues(unsigned int chain) const;
    StochasticNode *clone(std::vector<Node const *> const &parents,
			  Node const *lower, Node const *upper) const;
    unsigned int df() const;
};

#endif /* SCALAR_STOCHASTIC_NODE_H_ */


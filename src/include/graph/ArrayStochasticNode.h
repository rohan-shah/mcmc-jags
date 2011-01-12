#ifndef ARRAY_STOCHASTIC_NODE_H_
#define ARRAY_STOCHASTIC_NODE_H_

#include <graph/StochasticNode.h>

class ArrayDist;

/**
 * @short Array-valued Node defined by the BUGS-language operator ~
 */
class ArrayStochasticNode : public StochasticNode {
    ArrayDist const * const _dist;
    std::vector<std::vector<unsigned int> > _dims;
    void sp(double *lower, double *upper, unsigned int length, 
	    unsigned int chain) const;
public:
    /**
     * Constructs a new StochasticNode given an array-valued
     * distribution and an array of parent nodes, considered as
     * parameters to the distribution.
     */
    ArrayStochasticNode(ArrayDist const *dist,
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

#endif /* ARRAY_STOCHASTIC_NODE_H_ */


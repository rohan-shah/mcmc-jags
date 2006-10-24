#ifndef STOCHASTIC_NODE_H_
#define STOCHASTIC_NODE_H_

#include <graph/Node.h>

class Distribution;
class RNG;

/**
 * 
 */
class StochasticNode : public Node {
    Distribution const * const _dist;
    std::vector<std::vector<double const *> > _parameters;
    std::vector<std::vector<unsigned int> > _dims;
    Node const *_lower;
    Node const *_upper;
    /* Forbid copying of Stochastic Nodes */
    StochasticNode(StochasticNode const &orig);
    StochasticNode &operator=(StochasticNode const &rhs);
public:
    /** 
     * Constructs a new StochasticNode given a distribution, a set of
     * parameters and, optionally, upper and lower bounds. If bounds
     * are given then the distribution of the constructed StochasticNode
     * is truncated at the current value of the lower and upper bounds.
     */
    StochasticNode(Distribution const *dist, 
                   std::vector<Node const *> const &parameters,
                   Node const *lower=0, Node const *upper=0);
    ~StochasticNode();
    /**
     * Indicates whether the distribution of the node is bounded above
     * or below.
     */
    bool isBounded() const;
    /**
     * Returns a pointer to the lower bound if the distribution is
     * truncated, or a NULL pointer otherwise.
     */
    Node const *lowerBound();
    /**
     * Returns a pointer to the upper bound if the distribution is
     * truncated, or a NULL pointer otherwise.
     */
    Node const *upperBound();
    /**
     * Returns a pointer to the Distribution object of the StochasticNode.
     * @see Distribution
     */
    Distribution const *distribution() const;
    /**
     * Returns a vector of parameter values for the given chain.
     */
    std::vector<double const *> const &parameters(unsigned int chain) const;
    /**
     * FIXME
     */
    std::vector<std::vector<unsigned int> > const &parameterDims() const;
    /**
     * Returns the log of the prior density of the StochasticNode given
     * the current parameter values.
     */
    double logDensity(unsigned int chain) const;
    /**
     * Draws a random sample from the Distribution that defines the node
     * given the current parameter values and sets the Node to that value.
     * @param rng Random Number Generator object
     * @param chain Number of chain to modify (starting from zero).
     */
    void randomSample(RNG *rng, unsigned int chain);
    /**
     * A deterministic sample for a stochastic node sets it to a
     * "typical" value of the distribution. The exact behaviour
     * depends on the Distribution used to define the StochasticNode,
     * but it will usually be the prior mean, median, or mode.
     */
    void deterministicSample(unsigned int chain);
    /**
     * Stochastic nodes are always random variables. This function returns
     * true.
     */
    bool isVariable() const;
    
};

StochasticNode const *asStochastic(Node const *node);

#endif /* STOCHASTIC_NODE_H_ */

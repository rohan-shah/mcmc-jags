#ifndef STOCHASTIC_NODE_H_
#define STOCHASTIC_NODE_H_

#include <graph/Node.h>

class Distribution;
class RNG;

/**
 * @short Node defined by the BUGS-language operator ~
 */
class StochasticNode : public Node {
    Distribution const * const _dist;
    std::vector<std::vector<double const *> > _parameters;
    std::vector<std::vector<unsigned int> > _dims;
    Node const *_lower;
    Node const *_upper;
    unsigned int _nrep;
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
     * @see Distribution#checkParameterValue
     */
    bool checkParentValues(unsigned int chain) const;
    /**
     * Stochastic nodes are always random variables. This function returns
     * true.
     */
    bool isVariable() const;
    /**
     * Checks that the parameters are within the valid range determined
     * by the distribution.
     *
     * @param chain Chain number to check parameters for
     *
     * @see Distribution#checkParameterValue
     */
    bool checkParameterValue(unsigned int chain) const;    
    /**
     * A stochastic node may be replicated, if two or more identically
     * distributed variables are observed. The repCount records the
     * number of times a node has been observed. Newly allocated stochastic
     * Nodes have a repCount of 1.
     */
    unsigned int repCount() const;
    /**
     * Increments the repCount of a stochastic node by 1.
     */
    void replicate();
    /**
     * Stochastic nodes are never linear. This function always returns false.
     */
    bool isLinear(std::set<Node const*> const &parameters, bool fixed) const;
    /**
     * Stochastic nodes are never scale functions. This function
     * always returns false
     */
    bool isScale(std::set<Node const*> const &parameters, bool fixed) const;
};

StochasticNode const *asStochastic(Node const *node);
/**
 * Number of degrees of freedom of a node
 * @see Distribution#df
 */
unsigned int df(StochasticNode const *snode);

#endif /* STOCHASTIC_NODE_H_ */


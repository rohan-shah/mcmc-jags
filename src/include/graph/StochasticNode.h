#ifndef STOCHASTIC_NODE_H_
#define STOCHASTIC_NODE_H_

#include <graph/Node.h>
#include <distribution/Distribution.h>

class RNG;

/**
 * @short Node defined by the BUGS-language operator ~
 *
 * Stochastic nodes represent the random variables that are the
 * fundamental building blocks of a Bayesian hierarchical model. In
 * the BUGS language, they are defined on the left hand side of a
 * stochastic relation. For example, the relation 
 *
 * <pre>y ~ dnorm(mu, tau) T(L, U)</pre> 
 *
 * defines y to be a normally distributed random variable with parameters
 * mu,  tau, L, and U (mean, precision, lower bound, upper bound). The
 * last two parameters, defined by the T(,) construct, are optional. If
 * they are supplied, then the distribution of the node is truncated
 * to lie in the range (L, U). Not all distributions can be truncated.
 *
 * JAGS allows you to define stochastic nodes that are, in fact,
 * not random at all, but are deterministic functions of their parameters.
 * A common example is the dinterval distribution
 *
 * <pre>group[i] ~ dinterval(true[i], cutpoints[1:N])</pre>
 *
 * where the value of group[i] is determined by where the value of
 * true[i] falls in the vector of supplied cutpoints.  In this case,
 * the stochastic node leads a double life. If it is observed, then it
 * is considered a random variable, and generates a likelihood for its
 * stochastic parents.  If it is unobserved then it is treated as a
 * deterministic function of its parents, just as if it were a
 * LogicalNode.
 * 
 * @see Distribution
 */
class StochasticNode : public Node {
    Distribution const * const _dist;
    Node const *_lower;
    Node const *_upper;
    bool _observed;
    bool _discrete;
    virtual void sp(double *lower, double *upper, unsigned int length,
		    unsigned int chain) const = 0;
protected:
    std::vector<std::vector<double const*> > _parameters;
public:
    /**
     * Constructs a new StochasticNode given a distribution, a vector
     * of parent nodes, considered as parameteres to the distribution,
     * and, optionally, upper and lower bounds. If bounds are given
     * then the distribution of the constructed StochasticNode is
     * truncated at the value of the bounds. 
     */
    StochasticNode(std::vector<unsigned int> const &dim,
		   Distribution const *dist,
                   std::vector<Node const *> const &parameters,
		   Node const *lower, Node const *upper);
    ~StochasticNode();
    /**
     * Returns a pointer to the Distribution.
     */
    Distribution const *distribution() const;
    /**
     * Returns the log of the prior density of the StochasticNode
     * given the current parameter values.
     *
     * @param chain Number of chain (starting from zero) for which
     * to evaluate log density.
     *
     * @param type Indicates whether the full probability density
     * function is required (PDF_FULL) or whether partial calculations
     * are permitted (PDF_PRIOR, PDF_LIKELIHOOD). See PDFType for
     * details.
     */
    virtual double logDensity(unsigned int chain, PDFType type) const = 0;
    /**
     * Draws a random sample from the prior distribution of the node
     * given the current values of it's parents, and sets the Node
     * to that value.
     *
     * @param rng Random Number Generator object
     * @param chain Index umber of chain to modify
     */
    virtual void randomSample(RNG *rng, unsigned int chain) = 0;
    /**
     * Draws a truncated random sample from the prior distribution of
     * the node. The lower and upper parameters are pointers to arrays
     * that are assumed to be of the correct size, or NULL pointers if
     * there is no bound
     *
     * @param lower Optional lower bound
     * @param upper Optional upper bound
     */
    virtual void truncatedSample(RNG *rng, unsigned int chain,
				 double const *lower=0, 
				 double const *upper=0) = 0;
    /**
     * A deterministic sample for a stochastic node sets it to a
     * "typical" value of the prior distribution, given the current
     * values of its parents. The exact behaviour depends on the
     * Distribution used to define the StochasticNode, but it will
     * usually be the prior mean, median, or mode.
     */
    virtual void deterministicSample(unsigned int chain) = 0;
    /**
     * Stochastic nodes always represent random variables in the model.
     */
    bool isRandomVariable() const;
    /**
     * Writes the lower and upper limits of the support of a given
     * stochastic node to the supplied arrays. If the node has upper and
     * lower bounds then their values are taken into account in the
     * calculation.
     *
     * @param lower pointer to start of an array that will hold the lower 
     * limit of the support
     *
     * @param lower pointer to start of an array that will hold the upper 
     * limit of the support
     *
     * @param length size of the lower and upper arrays.
     *
     * @param chain Index number of chain to query
     */
    void support(double *lower, double *upper, unsigned int length,
		 unsigned int chain) const;
    double const *lowerLimit(unsigned int chain) const;
    double const *upperLimit(unsigned int chain) const;
    std::string deparse(std::vector<std::string> const &parameters) const;
    bool isDiscreteValued() const;
    bool isObserved() const;
    void setObserved();
    Node const *lowerBound() const;
    Node const *upperBound() const;
    /**
     * Creates a copy of the stochastic node.  Supplying the parents
     * of this node as the argument creates an identical copy.
     *
     * @param parents Parents of the cloned node. 
     */
    StochasticNode * clone(std::vector<Node const *> const &parents) const;
    virtual StochasticNode * 
	clone(std::vector<Node const *> const &parameters,
	      Node const *lower, Node const *upper) const = 0;
    virtual unsigned int df() const = 0;
    //Required for KL in dic
    std::vector<double const*> const &parameters(unsigned int chain) const;
};



/**
 * Returns true if the upper and lower limits of the support of
 * the stochastic node are fixed. Upper and lower bounds are taken
 * into account.
 */
bool isSupportFixed(StochasticNode const *snode);

/**
 * Indicates whether the distribution of the node is bounded
 * either above or below.
 */
bool isBounded(StochasticNode const *node);


#endif /* STOCHASTIC_NODE_H_ */


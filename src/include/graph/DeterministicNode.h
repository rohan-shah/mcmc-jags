#ifndef DETERMINISTIC_NODE_H_
#define DETERMINISTIC_NODE_H_

#include <graph/Node.h>

/**
 * @short Base class for deterministic Node objects
 *
 * The value of a deterministic node is exactly by the values of its parents.
 */
class DeterministicNode : public Node {
public:
    DeterministicNode(std::vector<unsigned int> const &dim,
		      std::vector<Node const *> const &parents);
    /**
     * Random samples from a Deterministic node are not random.
     * This function simply calculates the value of the node from its
     * parent values and leaves the RNG object untouched.
     */
    void randomSample(RNG*, unsigned int nchain);
    /**
     * Deterministic nodes are not random variables. This function
     * always returns false.
     */
    bool isVariable() const;
    /**
     * Tests whether the value of the node is a linear function of the
     * ancestor nodes X1, X2, ... Xn of form A + B %*% X1 + B2 %*% X2
     * + ...  + Bn %*% Xn. Preservation of linearity is a criterion
     * used by some Samplers to determine if they can act on a
     * set of stochastic nodes.
     *
     * False negative responses are permitted: i.e. the value false may
     * be returned when the node is, in fact, a linear function, but
     * false positives are not allowed.
     * 
     * @param parameters Set of all ancestors of this node that are
     * themselves linear functions functions of X1, X2, ... Xn.  The
     * set must include the ancestor nodes (as they are trivial linear
     * functions of themselves).  The current node may be a non-linear
     * function of nodes not in this set.
     *
     * @param fixed When true, the test is more stringent and returns
     * the value true only if the function is BOTH linear AND the
     * coefficients B1, ... Bn are fixed (but not necessarily A).
     */
    virtual bool 
	isLinear(std::set<Node const*> const &parameters, bool fixed) const = 0;
    /**
     * Tests whether the value of the node is a scale function of the
     * ancestor node X. A scale function is a trivial linear function
     * of the form A + B %*% X where either A or B is zero (or both).
     * Preservation of scale is used by some Samplers to determine
     * if they can act on a set of stochastic nodes.
     * 
     * @param parameters Set of all ancestors of the current node that
     * are themselves scale transformations of X. The set must include
     * X (as this is a trivial scale transformation of itself).
     *
     * @param fixed When true, the test is more stringent and returns
     * the value true only if the function is a scale transformation
     * with fixed coefficient B.
     */
    virtual bool 
	isScale(std::set<Node const*> const &parameters, bool fixed) const = 0;
};

#endif /* DETERMINISTIC_NODE_H_ */

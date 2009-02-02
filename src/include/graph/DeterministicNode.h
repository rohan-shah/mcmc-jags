#ifndef DETERMINISTIC_NODE_H_
#define DETERMINISTIC_NODE_H_

#include <graph/Node.h>

/**
 * @short Base class for deterministic Node objects
 *
 * The value of a deterministic node is exactly by the values of its
 * parents.
 */
class DeterministicNode : public Node {
public:
    DeterministicNode(std::vector<unsigned int> const &dim,
		      std::vector<Node const *> const &parents);
    ~DeterministicNode();
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
    bool isRandomVariable() const;
    /**
     * A deterministic node is observed if all its parents are
     */
    bool isObserved() const;
    /**
     * Tests whether the value of the node is a linear function of a
     * set of ancestor nodes X = (X1, ... Nn), i.e. whether the value
     * of the node can be expressed as A + B %*% X1 + B2 %*% X2 + ...
     * + Bn %*% Xn. Preservation of linearity is a criterion used by
     * some Samplers to determine if they can act on a set of
     * stochastic nodes.
     *
     * False negative responses are permitted: i.e. the value false may
     * be returned when the node is, in fact, a linear function, but
     * false positives are not allowed.
     * 
     * The test for linearity takes place in a graph, which is implicitly
     * defined by the linear_marks parameter (and may be accessed directly
     * using the GraphMarks#graph member function on linear_marks). Only
     * paths that are contained entirely inside the graph are considered.
     * The graph is assumed to be acyclic. Since the Node#isLinear function
     * is designed to be called iteratively on a sequence of nodes, it
     * relies on a GraphMarks object for book-keeping.
     *
     * @param linear_marks A GraphMarks object in which all ancestors
     * of the current node that are also descendants of X have been
     * marked.  The mark values are MARK_TRUE if the node is a
     * (possibly fixed) linear function of X, and MARK_FALSE
     * otherwise.  Ancestors of the current node that are not
     * descendants of X should remain unmarked.
     *
     * @param fixed Logical flag. When true, the test is more
     * stringent and the isLinear function returns the value true only
     * if the current node is a fixed linear function of X, i.e. the
     * coefficients B1, ... Bn are constant (but not necessarily
     * A). In this case, the linear_marks parameter must also conform
     * to the more stringent conditions: ancestors of the current node
     * should be marked with MARK_TRUE only if they are fixed linear
     * functions of X. They should be marked with MARK_FALSE if they
     * are non-linear or non-fixed linear functions of X.
     */
    virtual bool 
	isLinear(GraphMarks const &linear_marks, bool fixed) const = 0;
    /**
     * Tests whether the value of the node is a scale function of the
     * ancestor node X. A scale function is a trivial linear function
     * of the form A + B %*% X where either A or B is zero (or both).
     * Preservation of scale is used by some Samplers to determine
     * if they can act on a set of stochastic nodes.
     * 
     * The isScale function works the same way as the isLinear function.
     *
     * @param scale_marks. GraphMarks object in which all ancestors
     * of the current node that are also descendants of X are marked
     * with MARK_TRUE or MARK_FALSE, depending on whether they are
     * (possibly fixed) scale functions of X or not.
     *
     * @param fixed When true, the test is more stringent and returns
     * the value true only if the function is a scale transformation
     * with fixed coefficient B. In this case, the scale_marks parameter
     * must conform to the more stringent conditions: only fixed scale
     * functions of X may be marked with MARK_TRUE
     */
    virtual bool 
	isScale(GraphMarks const &scale_marks, bool fixed) const = 0;
};

#endif /* DETERMINISTIC_NODE_H_ */

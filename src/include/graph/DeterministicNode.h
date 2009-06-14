#ifndef DETERMINISTIC_NODE_H_
#define DETERMINISTIC_NODE_H_

#include <graph/Node.h>

/**
 * @short Closed classes of deterministic nodes
 *
 * A deterministic node may be considered as a function of any of its
 * ancestors.  Some classes of function are closed under composition:
 * if g() and f() both belong to the class, then so does g(f()). This
 * property is used by some Samplers to determine whether a node, or
 * block of nodes, can be sampled.
 *
 * DNODE_LINEAR denotes a linear function of the ancestor nodes X =
 * (X1, ... Nn). A linear function takes the form A + B %*% X1 + B2
 * %*% X2 + ... * + Bn %*% * Xn. 
 *
 * DNODE_SCALE denotes a scale function of the single ancestor node
 * X. This is a trivial linear function of the form A + B * X * where
 * either A or B is zero (or both). 
 *
 * DNODE_POWER denostes a power function of the single ancestor node1
 * X.  This is a function of the form A * X^B.  Power functions become
 * linear functions if we transform to a log scale: i.e. if Y=A*X^B
 * and A > 0, then log(Y) is a linear function of log(X).
 *
 * A function is considered to be a fixed function if the coefficient
 * B, or coefficients B=(B1...Bn) is fixed. 
 *
 * @see DeterministicNode#isClosed
 */
enum ClosedFuncClass {DNODE_LINEAR, DNODE_SCALE, DNODE_POWER};

/**
 * @short Base class for deterministic Node objects
 *
 * The value of a deterministic node is determined exactly by the
 * values of its parents.
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
     * Tests whether the value of the node is belongs to a closed
     * class when considered as a function of a given ancestor node X,
     * or nodes X1 ... Xn.
     *
     * False negative responses are permitted: i.e. the value false
     * may be returned when the node is, in fact, in the closed class,
     * but false positives are not allowed.
     *
     * A pre-condition is that all nodes on the path from the ancestor
     * node X to the current node must be in the closed class. Thus,
     * the isClosed function should be called iteratively, and if it
     * returns false for any ancestor then you can infer that this
     * node is not in the closed class without calling the isClosed
     * function.
     *
     * @param ancestors Set containing all ancestors of the test node
     * that are in the closed class.
     *
     * @param dnfunc Closed class to be tested.
     *
     * @param fixed When true, the closed class is restricted to the
     * sub-class of functions that are considered fixed.
     */
    virtual bool isClosed(std::set<Node const *> const &ancestors, 
			  ClosedFuncClass fc, bool fixed) const = 0;
};

#endif /* DETERMINISTIC_NODE_H_ */

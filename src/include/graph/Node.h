#ifndef NODE_H_
#define NODE_H_

#include <set>
#include <vector>
#include <string>

class NodeNameTab;
class RNG;

/**
 * @short Node in a directed acyclic graph representing a probability 
 * distribution
 *
 * Nodes are reference managed and will delete themselves when the
 * reference count reaches zero.  Referencing and dereferencing
 * takes place automatically when a node is inserted into/removed
 * from a graph, but can also be done by calling the ref and
 * unref member functions.
 *
 * Nodes should be dynamically allocated, and should be inserted
 * into a Graph as soon as they are created.  
 *
 * @see Graph
 */
class Node {
    std::vector<Node const *> _parents;
    std::set<Node*> *_children;
    unsigned int _ref;
    bool _isobserved;
    bool _isdiscrete;

    /* Forbid copying of Node objects */
    Node(Node const &orig);
    Node &operator=(Node const &rhs);
protected:
    const std::vector<unsigned int> _dim;
    const unsigned int _length;
    const unsigned int _nchain;
    double *_data;
  
public:
    /**
     * Constucts a Node with no parents.
     * @param dim Dimension of new Node.
     * @param nchain Number of chains that the Node will contain data on.
     */
    Node(std::vector<unsigned int> const &dim, unsigned int nchain);
    /**
     * Constructs a node with parents.  Each parent must contain data on
     * the same number of chains. Subclasses of Node may give specific
     * meaning to the ordering of the parents.
     *
     * @ param dim Dimension of new Node.
     *
     * @ param parents vector of parent nodes. A node may not be its own
     * parent.
     */
    Node(std::vector<unsigned int> const &dim, 
	 std::vector<Node const *> const &parents);
    /**
     * Destructor. 
     */
    virtual ~Node();
    /**
     * Increments the reference count.
     */
    void ref();
    /**
     * Decrements reference count. The node deletes itself when the
     * reference count is zero.
     */
    void unref();
    /**
     * Shows the current reference count.
     */
    unsigned int refCount() const;
    /**
     * Number of chains.
     */ 
    unsigned int nchain() const;
    /**
     * Vector of parents.
     */
    std::vector<Node const *> const &parents() const;
    /**
     * Set of children.
     *
     * This is not a constant member function of the Node class for a 
     * good reason.  Even if it were a constant function, you could still
     * access a non-constant pointer to this node by looking among
     * the children of its parents. To preserve the spirit of const
     * correctness we forbid access to the children of a Node when we
     * are given a constant pointer or reference to it.
     */
    std::set<Node*> const *children();
    /**
     * Draws a random sample from the node's prior distribution.
     * @param rng Pointer to random number generator
     * @param chain Number of chain from which to draw sample
     */
    virtual void randomSample(RNG *rng, unsigned int chain) = 0;
    /**
     * Calculates a value for the node based on its parents' values.
     * @param chain Number of chain from which to draw sample
     */
    virtual void deterministicSample(unsigned int chain) = 0;
    /**
     * Checks whether the parents of the Node have valid values.
     */
    virtual bool checkParentValues(unsigned int chain) const = 0;
    /**
     * Initializes the node for the given chain. The value vector of a
     * newly constructed Node consists of missing values.  This
     * function sets the value of the node by forward sampling from
     * its parents.  If the Node has previously had its value set, the
     * function will do nothing and return the value true.
     * Initialization will fail if any of the parent nodes is
     * uninitialized, and in this case the return value is false.
     *
     * @param rng random number generator
     * @param n chain number
     *
     * @returns a logical value indicating success
     */
    bool initialize(RNG *rng, unsigned int n);
    /**
     * Initializes a node, in all chains, if it is not a random
     * variable and if all of its parents are observed. In this case,
     * the value of the node is also fixed. Otherwise the function
     * has no effect.
     * @see initialize
     */
    void initializeData();
    /**
     * Returns the name of the node.  The default implementation looks up
     * the node in the supplied name table and returns an empty string if it
     * is not found. Subclasses can overload this function and try to calculate
     * the Node name based on the names of its parents.
     *
     * @param name_table Lookup table for node names.
     */
    virtual std::string name(NodeNameTab const &name_table) const;
    /**
     * Returns true if the node is to be considered a random variable.
     */
    virtual bool isVariable() const = 0;
    /**
     * Sets the value of the node, in all chains, to "value", and marks
     * the node as "observed". This function can only be called once for
     * a given Node.
     * @param value array of doubles
     * @param length length of the value array
     */
    void setObserved(double const *value, unsigned int length);
    /**
     * Indicates whether node is observed. 
     */
    bool isObserved() const;
    /**
     * Sets the value of the node for a given chain
     * @param value Array of values to be assigned
     * @param length Length of the value array
     * @param chain number of chain (starting from zero) to modify
     *
     * @see SArray#setValue
     */
    void setValue(double const *value, unsigned int length, unsigned int chain);
    /**
     * Indicates whether a node is discrete-valued or not.
     * @see SArray#isDiscreteValued
     */
    bool isDiscreteValued() const;
    /**
     * Permanently sets the node to be discrete-valued for all chains.
     * @see SArray#isDiscreteValued
     */
    void setDiscreteValued();
    /**
     * Returns a pointer to the start of the array of values for 
     * the given chain.
     */
    double const *value(unsigned int chain) const;
    /**
     * Returns the length of the value array
     */
    unsigned int length() const;
    /**
     * Returns the dimensions of the Node
     */
    std::vector<unsigned int> const &dim() const;
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

/**
 * Calculate the number of chains of parameter vector. Returns 0
 * if the parameters are inconsistent
 */
unsigned int countChains(std::vector<Node const *> const &parameters);

#endif /* NODE_H_ */

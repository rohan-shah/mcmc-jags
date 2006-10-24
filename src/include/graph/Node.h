#ifndef NODE_H_
#define NODE_H_

#include <sarray/Range.h>

#include <set>
#include <vector>
#include <string>

class NodeNameTab;
class RNG;

/**
 * Abstract base class for nodes in a directed graphical model.
 *
 * Nodes are reference managed and will delete themselves when the
 * reference count reaches zero.  Referencing and dereferencing
 * takes place automatically when a node is inserted into/removed
 * from a graph, but can also be done by calling the ref() and
 * unref() member functions.
 *
 * Nodes should be dynamically allocated, and should be inserted
 * into a Graph as soon as they are created.  
 *
 * @see Graph
 * @short Node in a directed graphical model
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
   */
  std::set<Node*> const *children() const;
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
   * Attempts to initialize the node. If the parents are observed and
   * the node is non-variable, its value is fixed.
   *
   * Initialization will fail if any parents are non-initialized. 
   * An unobserved node that does not represent a variable in the model
   * may be re-initialized. For other nodes, reinitialization will
   * have no effect.
   *
   * @returns a logical value indicating success
   */
  bool initialize();
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
   * Range of indices of the node
   */
  Range const &range() const;
  /**
   * Sets the value of the node, in all chains, to "value", and marks
   * the node as "observed". This function can only be called once for
   * a given Node.
   * @param value array of doubles
   * @param length length of the value array
   */
  void setObserved(double const *value, unsigned int length);
  /**
   * Indicates whether node is observed. This is only true if a call
   * to setObserved has previously been used to set the value.
   */
  bool isObserved() const;
  /**
   * Sets the value of the node for a given chain
   * @param value Array of values to be assigned
   * @param length Length of the value array
   * @param chain number of chain (starting from zero) to modify
   *
   * @see SArray##setValue
   */
  void setValue(double const *value, unsigned int length, unsigned int chain);
  /**
   * Indicates whether a node is discrete-valued or not.
   * @see SArray##isDiscreteValued
   */
  bool isDiscreteValued() const;
  /**
   * Permanently sets the node to be discrete-valued for all chains.
   * @see SArray##isDiscreteValued
   */
  void setDiscreteValued();
  /**
   * Gives direct access to the value of the node for a given chain
   * without going through the data member function.
   * @see Node##data
   */
  double const *value(unsigned int chain) const;
  std::vector<unsigned int> const &dim() const;
  unsigned int length() const;
};

/**
 * Calculate the number of chains of parameter vector. Returns 0
 * if the parameters are inconsistent
 */
unsigned int countChains(std::vector<Node const *> const &parameters);

#endif /* NODE_H_ */

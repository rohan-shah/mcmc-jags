#ifndef NODE_ARRAY_H_
#define NODE_ARRAY_H_

#include <graph/Graph.h>
#include <sarray/Range.h>

#include <string>
#include <map>

class SArray;
class Model;

/**
 * @short Multi-dimensional array that can be tiled with Node objects
 * 
 * A NodeArray is a container class for nodes with a dimension
 * attribute.  The array can be tiled with nodes using the insert
 * function.  Inserted nodes can be retrieved using the find
 * function.  Arbitrary subsets of the NodeArray can be returned with
 * the getSubSet function.
 */
class NodeArray {
  std::string _name;
  Range _range;
  Graph _member_graph;
  unsigned int _nchain;
  Node **_node_pointers;
  unsigned int *_offsets;
  std::map<Range, Node *> _generated_nodes;

  /* Forbid copying */
  NodeArray(NodeArray const &orig);
  NodeArray &operator=(NodeArray const &rhs);

  bool findActiveIndices(std::vector<unsigned int> &ind, unsigned int k,
                         std::vector<int> const &lower, std::vector<unsigned int> const &dim) const;
public:
  /**
   * Constructor. Creates a NodeArray with the given name and dimension
   */
  NodeArray(std::string const &name, std::vector<unsigned int> const &dim, 
	    unsigned int nchain);
  ~NodeArray();
  /**
   * Inserts a node into the subset of the NodeArray defined by range.
   * The dimension of the node must conform with the given range.
   * The given range must not overlap any previously inserted node.
   *
   * The node is added to an internal graph. 
   *
   * @exception runtime_error
   */
  void insert(Node *node, Range const &range);
  /**
   * Determines whether the given range is empty of inserted nodes,
   * and hence whether it can be used as an argument to insert
   */
  bool isEmpty(Range const &range) const;
  /**
   * Returns a node corresponding to the given range.  The range must
   * have been used in a previous call to insert, otherwise a NULL
   * pointer is returned.
   */
  Node* find(Range const &range) const;
  /**
   * Returns an arbitrary subset of the NodeArray. If the range
   * corresponds to a previously inserted node, this will be
   * returned. Otherwise, an aggregate node will be created, and it
   * will be added to the given model.  If the range is not completely
   * covered by inserted nodes, a NULL pointer will be returned.
   */
  Node* getSubset(Range const &range, Model &model);
  /**
   * Sets the values of the nodes in the array. 
   *
   * @param value SArray containing values to be used. The value
   * vector of the SArray may contain missing values.  If so, then the
   * part of the value vector corresponding to each inserted node must be
   * either completely missing (in which no action is taken) or
   * contain no missing values.
   *
   * @param value array with dimensions matching the NodeArray containing
   * values.
   *
   * @param chain Index number of chain to which to apply values.
   */
  void setValue(SArray const &value, unsigned int chain);
  /**
   * Gets the values of selected nodes that have been inserted into
   * the array.
   *
   * @param value SArray to which values should be written.
   *
   * @param chain Index number of chain to read.
   *
   * @param condition.  Boolean function that returns true for nodes
   * whose values are to be read.
   */
  void getValue(SArray &value, unsigned int chain,
		bool (*condition)(Node const *)) const;
  /**
   * Set data, creating a constant node for any non-missing value.  An
   * exception is thrown if any of the non-missing values corresponds
   * to an existing node in the array.
   */
  void setData(SArray const &value, Model *model);
  /**
   * Returns the name of the node array
   */
  std::string const &name() const;
  /**
   * Returns the range of indices covered by the NodeArray
   */
  Range const &range() const;
  /**
   * Returns the range corresponding to the given node, if it
   * belongs to the graph associated with the NodeArray. If it is
   * not in the graph, a NULL Range is returned.
   */
  Range getRange(Node const *node) const;
  /**
   * Returns the number of chains of the nodes stored in the array
   */
  unsigned int nchain() const;
};


#endif /* NODE_ARRAY_H */

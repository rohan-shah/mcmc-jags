#ifndef STOCHASTIC_FACTORY_H_
#define STOCHASTIC_FACTORY_H_

#include <graph/Graph.h>
#include <graph/Node.h>
#include <sarray/SArray.h>

#include <vector>
#include <utility>
#include <map>
#include <cfloat>

class Distribution;
class StochasticNode;

/**
 * @short Uniquely defines an observed StochasticNode 
 *
 * A "stochastic trio" consists of a function, a vector of arguments,
 * and a data value. These three elements uniquely define an observed
 * StochasticNode, so that two nodes with the same definition can be
 * represented by the same StochasticNode object.
 *
 * @see StochasticFactory
 */
struct StochasticTrio {
    Distribution const *dist;
    std::vector<Node const*> param;
    SArray data;
    StochasticTrio(Distribution const *dist,
                   std::vector<Node const*> const &param,
                   SArray const &data);
};


/**
 * @short "Less than" operator for StochasticTrio
 *
 * Defines a unique, but arbitrary, ordering of StochasticTrio objects.
 * The only use of this function is for the STL map class with StochasticTrio
 * as a key.
 * 
 * Two StochasticTrio objects A and B are considered to be equivalent
 * (i.e. lt(A,B) and lt(B,A) are both false) if they have the same
 * distribution, the same vector of parameters and the same data
 * (within numerical tolerence)
 */
bool lt(StochasticTrio const &arg1, StochasticTrio const &arg2);


/**
 * @short function object for the STL map class using StochasticTrio as a key
 */
struct ltstochastic
{
  bool operator()(StochasticTrio const &arg1, StochasticTrio const &arg2) const
  {
    return lt(arg1, arg2);
  }
};


/**
 * @short Factory for observed StochasticNode objects 
 * 
 * The likelihood of an observed stochastic node is determined
 * uniquely by its distribution, parameters, and data value.  The
 * purpose of a StochasticFactory is to avoid unnecessary duplication
 * of observed stochastic nodes by having a factory object for them
 * that will create and/or lookup stochastic nodes based on these
 * arguments.
 */
class StochasticFactory 
{ 
  std::map<StochasticTrio, StochasticNode*, ltstochastic> _stochasticmap;
	
public:
  /**
   * Gets an observed stochastic node with a given distribution,
   * parameters and data. The results are cached, so if a request is
   * repeated, the same node will be returned. If a newly allocated
   * node is returned, it is also added to the given Graph.
   */
  StochasticNode *getStochasticNode(Distribution const *dist, 
				    std::vector<Node const*> const &param,
				    SArray const &data, Graph &graph);
};

#endif /* STOCHASTIC_FACTORY_H_ */

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
 * A "stochastic trio", consisting of a function, a vector of
 * arguments, and a data value uniquely defines an observed stochastic
 * node.
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
 * "Less than" operator for StochasticTrios. Two StochasticTrios A and
 * B are considered to be equivalent (i.e. lt(A,B) and lt(B,A) are
 * both false) if they have the same distribution, the same vector of
 * parameters and the same data (within numerical tolerence)
 *
 * For non-equivalent StochasticTrios, the ordering is unique, but 
 * arbitrary. The only use of this function is for the STL map class
 * with StochasticTrio as key.
 */
bool lt(StochasticTrio const &arg1, StochasticTrio const &arg2);
/**
 * Function object for the STL map class using StochasticTrio as a key
 */
struct ltstochastic
{
  bool operator()(StochasticTrio const &arg1, StochasticTrio const &arg2)
  {
    return lt(arg1, arg2);
  }
};

/**
 * @short Factory object for observed stochastic nodes
 * 
 * The likelihood of an observed stochastic node is determined
 * uniquely by its distribution, parameters, and data value.  The
 * purpose of a StochasticFactory is to avoid unnecessary duplication
 * of stochastic nodes by having a factory object for them that will
 * create and/or lookup stochastic nodes based on these arguments.
 */
class StochasticFactory 
{ 
  std::map<StochasticTrio, StochasticNode*, ltstochastic> _stochasticmap;
	
public:
  /**
   * Get an observed stochastic node with a given distribution, parameters
   * and data. The results are cached, so if a request is repeated,
   * the same node will be returned. If a newly allocated node is
   * returned, it is also added to the given Graph.
   */
  StochasticNode *getStochasticNode(Distribution const *dist, 
				    std::vector<Node const*> const &param,
				    SArray const &data, Graph &graph);
};

#endif /* STOCHASTIC_FACTORY_H_ */

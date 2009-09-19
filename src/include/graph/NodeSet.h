#ifndef NODE_SET_H_ 
#define NODE_SET_H_

#include <set>

class StochasticNode;

/** 
 * Function object that extends less than relationships to pointers
 * to stochastic nodes.
 */
struct less_snode {
    bool operator()(StochasticNode const *lhs, StochasticNode const *rhs) const;
};

typedef std::set<StochasticNode *, less_snode> StochasticNodeSet;
typedef std::set<StochasticNode const *, less_snode> ConstStochasticNodeSet;

#endif /* NODE_SET_H_ */

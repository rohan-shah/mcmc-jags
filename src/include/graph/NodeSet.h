#ifndef NODE_SET_H_ 
#define NODE_SET_H_

#include <set>

class StochasticNode;

typedef std::set<StochasticNode *> StochasticNodeSet;
typedef std::set<StochasticNode const*> ConstStochasticNodeSet;

#endif /* NODE_SET_H_ */

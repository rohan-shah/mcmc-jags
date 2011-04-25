#ifndef REAL_DSUM_H_
#define REAL_DSUM_H_

#include "RWDSum.h"

#include <vector>

class StochasticNode;
class GraphView;

namespace bugs {

/**
 * @short Sample parents of dsum nodes
 */
class RealDSum : public RWDSum
{
public:
    RealDSum(GraphView const *gv, unsigned int chain);
    void step(std::vector<double> &value, unsigned int nrow, 
	      unsigned int ncol, double s, RNG *rng) const;
    std::string name() const;
};

}

#endif /* REAL_DSUM_H_ */

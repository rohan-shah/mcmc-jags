#ifndef DISCRETE_DSUM_H_
#define DISCRETE_DSUM_H_

#include <sampler/RWMetropolis.h>

#include <vector>

class StochasticNode;
class GraphView;

/**
 * @short Sample parents of dsum nodes
 */
class DiscreteDSum : public RWMetropolis
{
    GraphView const *_gv;
    unsigned int _chain;
    unsigned int _nrep;
    std::vector<int> _ivalue;
public:
    DiscreteDSum(GraphView const *gv, unsigned int chain, unsigned int nrep);
    void step(std::vector<double> &value, double s, RNG *rng) const;
    std::string name() const;
    double logDensity() const;
    void getValue(std::vector<double> &value) const;
    void setValue(std::vector<double> const &value);
    static bool canSample(std::vector<StochasticNode *> const &nodes, 
			  Graph const &graph);
};

#endif /* DISCRETE_DSUM_H_ */

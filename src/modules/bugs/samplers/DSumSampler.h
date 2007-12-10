#ifndef DSUM_SAMPLER_H_
#define DSUM_SAMPLER_H_

#include <sampler/Slicer.h>

#include <vector>
#include <set>

class StochasticNode;
class DeterministicNode;
class Node;

/**
 * @short Sample parents of dsum nodes
 */
class DSumMethod : public Slicer
{
    double _x;
    long _sum;
public:
    DSumMethod();
    ~DSumMethod();
    void initialize(DensitySampler *sampler, unsigned int chain);
    void update(RNG *);
    void setValue(double x);
    double value() const;
    void getLimits(double *lower, double *upper) const;
    std::string name() const;
    static bool canSample(std::vector<StochasticNode *> const &nodes, 
			  Graph const &graph);
};

#endif /* DSUM_SAMPLER_H_ */

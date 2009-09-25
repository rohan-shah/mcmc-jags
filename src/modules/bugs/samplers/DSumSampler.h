#ifndef DSUM_SAMPLER_H_
#define DSUM_SAMPLER_H_

#include <sampler/Slicer.h>

#include <vector>
#include <set>

class StochasticNode;
class DeterministicNode;
class Node;
class Updater;

/**
 * @short Sample parents of dsum nodes
 */
class DSumMethod : public Slicer
{
    Updater const *_updater;
    unsigned int _chain;
    double _x;
    long _sum;
public:
    DSumMethod(Updater const *updater, unsigned int chain);
    ~DSumMethod();
    void update(RNG *);
    void setValue(double x);
    double value() const;
    void getLimits(double *lower, double *upper) const;
    std::string name() const;

    static bool canSample(std::vector<StochasticNode *> const &nodes, 
			  Graph const &graph);
    double logDensity() const;
};

#endif /* DSUM_SAMPLER_H_ */

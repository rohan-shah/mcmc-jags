#ifndef DSUM_METHOD_H_
#define DSUM_METHOD_H_

#include <sampler/Slicer.h>

#include <vector>
#include <set>

class StochasticNode;
class DeterministicNode;
class GraphView;

/**
 * @short Sample parents of dsum nodes
 */
class DSumMethod : public Slicer
{
    GraphView const *_gv;
    unsigned int _chain;
    double _x;
    long _sum;
public:
    DSumMethod(GraphView const *gv, unsigned int chain);
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

#endif /* DSUM_METHOD_H_ */

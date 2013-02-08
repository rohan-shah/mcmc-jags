#ifndef DSUM_METHOD_H_
#define DSUM_METHOD_H_

#include <sampler/Slicer.h>

#include <vector>
#include <set>

namespace jags {

class StochasticNode;
class DeterministicNode;
class GraphView;

namespace bugs {

/**
 * @short Sample parents of dsum nodes
 */
class DSumMethod : public Slicer
{
    GraphView const *_gv;
    unsigned int _chain;
    double _sum;
    bool _discrete;
    double _x;
public:
    DSumMethod(GraphView const *gv, unsigned int chain);
    ~DSumMethod();
    void update(RNG *);
    void setValue(double x);
    double value() const;
    void getLimits(double *lower, double *upper) const;
    std::string name() const;
    double logDensity() const;
    static bool canSample(std::vector<StochasticNode *> const &nodes, 
			  Graph const &graph);
};

}}

#endif /* DSUM_METHOD_H_ */

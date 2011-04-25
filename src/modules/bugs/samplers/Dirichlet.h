#ifndef DIRICHLET_METROPOLIS_H_
#define DIRICHLET_METROPOLIS_H_

#include <sampler/RWMetropolis.h>
#include <sampler/GraphView.h>

namespace bugs {

class DirchMetropolis : public RWMetropolis
{
    GraphView const *_gv;
    unsigned int _chain;
    double _s;
public:
    DirchMetropolis(GraphView const *gv, unsigned int chain);
    void getValue(std::vector<double> &x) const;
    void setValue(std::vector<double> const &x);
    void step(std::vector<double> &x, double size, RNG *rng) const;
    double logJacobian(std::vector<double> const &x) const;
    double logDensity() const;
    std::string name() const;
};

}

#endif /* DIRICHLET_METROPOLIS_H_ */

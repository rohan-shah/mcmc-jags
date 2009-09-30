#ifndef DIRICHLET_METROPOLIS_H_
#define DIRICHLET_METROPOLIS_H_

#include <sampler/RWMetropolis.h>

class Updater;

class DirchMetropolis : public RWMetropolis
{
    Updater const *_updater;
    unsigned int _chain;
    double _S;
public:
    DirchMetropolis(Updater const *updater, unsigned int chain);
    void getValue(std::vector<double> &x) const;
    void setValue(std::vector<double> const &x);
    void step(std::vector<double> &x, double size, RNG *rng) const;
    double logJacobian(std::vector<double> const &x) const;
    double logDensity() const;
    std::string name() const;
};

#endif /* DIRICHLET_METROPOLIS_H_ */

#ifndef MNORM_METROPOLIS_H_
#define MNORM_METROPOLIS_H_

#include <sampler/Metropolis.h>

class GraphView;

namespace bugs {

class MNormMetropolis : public Metropolis
{
    GraphView const *_gv;
    unsigned int _chain;
    double *_mean;
    double *_var;
    double *_prec;
    unsigned int _n;
    unsigned int _n_isotonic;
    double _sump, _meanp;
    double _lstep;
    unsigned int _nstep;
    unsigned int _p_over_target;
public:
    MNormMetropolis(GraphView const *gv, unsigned int chain);
    ~MNormMetropolis();
    void rescale(double p);
    void update(RNG *rng);
    bool checkAdaptation() const;
    std::string name() const;
    void getValue(std::vector<double> &value) const;
    void setValue(std::vector<double> const &value);
};

}

#endif /* MNORM_METROPOLIS_H_ */

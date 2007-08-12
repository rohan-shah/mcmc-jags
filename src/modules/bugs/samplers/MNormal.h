#ifndef MNORM_SAMPLER_H_
#define MNORM_SAMPLER_H_

#include <sampler/Metropolis.h>

class ParallelDensitySampler;

class MNormUpdate : public Metropolis
{
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
    MNormUpdate(std::vector<StochasticNode*> const &nodes);
    ~MNormUpdate();
    void rescale(double p);
    void transform(double const *v, unsigned int length,
		   double *nv, unsigned int nlength) const;
    void untransform(double const *nv, unsigned int nlength,
		     double *v, unsigned int length) const;
    void update(RNG *rng);
    bool checkAdaptation() const;
};

#endif /* MNORM_SAMPLER_H_ */

#ifndef MNORM_SAMPLER_H_
#define MNORM_SAMPLER_H_

#include <sampler/Metropolis.h>

class MNormSampler : public Metropolis
{
    double *_mean;
    double *_var;
    double *_prec;
    unsigned int _n;
    unsigned int _n_isotonic;
    double _sump;
    double _lstep;
    unsigned int _nstep;
    unsigned int _p_over_target;
public:
    MNormSampler(StochasticNode * node, 
		 Graph const &graph, unsigned int chain, 
		 double const *value, unsigned int length);
    ~MNormSampler();
    void rescale(double p, bool accept);
    void transformValues(double const *v, unsigned int length,
			 double *nv, unsigned int nlength) const;
    void update(RNG *rng);
};

#endif /* MNORM_SAMPLER_H_ */

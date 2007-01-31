#ifndef MNORMAL_SAMPLER_H_
#define MNORMAL_SAMPLER_H_

#include <config.h>
#include <sampler/Metropolis.h>

class MNormalSampler : public Metropolis
{
    double *_mean;
    double *_var;
    unsigned int _n;
    double _sump;
    double _lstep;
public:
    MNormalSampler(StochasticNode * node, 
		   Graph const &graph, unsigned int chain, 
		   double const *value, unsigned int length);
    void rescale(double p, bool accept);
    void transformValues(double const *v, unsigned int length,
			 double *nv, unsigned int nlength) const;
    void update(RNG *rng);
};

#endif /* MNORMAL_SAMPLER_H_ */

#ifndef MIX_SAMPLER_H_
#define MIX_SAMPLER_H_

#include <sampler/RWMetropolis.h>

class MixSampler : public RWMetropolis
{
    const unsigned int _nlevels;
    const double _log_min_power;
    std::vector<double> scale;
public:
    MixSampler(vector<StochasticNode *> const &snodes, Graph const &graph, 
	       unsigned int chain, unsigned int nlevels, double min_power);
    
    void update(RNG *rng);
    void setValue(double const *value, unsigned int length);
};

#endif /* MIX_SAMPLER_H_ */

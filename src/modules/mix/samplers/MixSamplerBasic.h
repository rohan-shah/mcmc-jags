#ifndef MIX_SAMPLER_BASIC_H_
#define MIX_SAMPLER_BASIC_H_

#include <sampler/Metropolis.h>

/* Sampler for mixture nodes without simulated tempering */
class MixSamplerBasic : public Metropolis
{
    const double _target_prob;
    double *_lower;
    double *_upper;
    double _lstep;
    bool _p_over_target;
    unsigned int _n;
public:
    MixSamplerBasic(std::vector<StochasticNode *> const &snodes,
                    Graph const &graph,
                    unsigned int chain, double const *value,
                    unsigned int length, double target_prob = 0.234);
    ~MixSamplerBasic();
    void update(RNG *rng);
    void rescale(double prob, bool accept);
    void transformValues(double const *v, unsigned int length,
                                     double *nv, unsigned int nlength) const;
    static bool canSample(std::vector<StochasticNode *> const &snodes, 
			  Graph const &graph);
    static void readValues(std::vector<StochasticNode *> const &snodes,
                           unsigned int chain, double *value,
                           unsigned int length);
    bool checkAdaptation() const;
};

#endif /* MIX_SAMPLER_BASIC_H_ */

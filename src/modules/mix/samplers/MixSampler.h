#ifndef MIX_SAMPLER_H_
#define MIX_SAMPLER_H_

#include <sampler/Metropolis.h>

class MixSampler : public Metropolis
{
    const unsigned int _nlevels;
    const double _log_min_power;
    const double _target_prob;
    double *_lower;
    double *_upper;
    std::vector<unsigned int> _n;
    std::vector<double> _lscale;
    std::vector<bool> _p_over_target;
    double _global_lscale;
    double _global_p_over_target;
    unsigned int _global_n;
    bool _temper;
    void localRescale(double prob, unsigned int t);
public:
    MixSampler(std::vector<StochasticNode *> const &snodes, Graph const &graph,
               unsigned int chain, double const *value, unsigned int length,
	       unsigned int nlevels=50, 
               double min_power=0.01, double target_prob = 0.234);
    ~MixSampler();
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

#endif /* MIX_SAMPLER_H_ */

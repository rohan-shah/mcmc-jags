#ifndef MIX_SAMPLER_H_
#define MIX_SAMPLER_H_

#include <sampler/Metropolis.h>

class MixSampler : public Metropolis
{
    const unsigned int _max_level;
    const double _delta; 
    unsigned int _level;
    double *_lower;
    double *_upper;
    unsigned int _n;
    std::vector<double> _lstep;
    std::vector<double> _pmean;
    bool _temper;
    
public:
    MixSampler(std::vector<StochasticNode *> const &snodes, Graph const &graph,
               unsigned int chain, double const *value, unsigned int length,
	       unsigned int max_level=50, double delta = 0.10);
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

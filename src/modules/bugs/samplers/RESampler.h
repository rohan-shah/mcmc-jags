#ifndef RANDOM_EFFECT_SAMPLER_H_
#define RANDOM_EFFECT_SAMPLER_H_

#include <sampler/Metropolis.h>

#include <vector>
#include <set>

class StochasticNode;
class DeterministicNode;
class Node;

namespace jags {
namespace bugs {

enum REMode {RE_SHUFFLE=0, RE_SCALE, RE_LOCATION, RE_INIT};

/**
 * @short Sample random effects
 */
class RESampler : public Metropolis
{
    REMode _mode;
    double _lower, _upper;
    double _lscale[3];
    bool _p_over_target[3];
    double _n[3];
public:
    RESampler(StochasticNode *variance, 
	      std::vector<StochasticNode *> const &effects,
	      Graph const &graph, unsigned int chain,
	      double const *initial_value, unsigned int length);
    ~RESampler();
    void update(RNG*);
    void rescale(double);
    void transformValues(const double*, unsigned int, double*, unsigned int) 
	const;
    bool checkAdaptation() const;
    std::string name() const;
};

}}

#endif /* RANDOM_EFFECT_SAMPLER_H_ */

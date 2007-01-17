#ifndef RW_METROPOLIS_H_
#define RW_METROPOLIS_H_

#include <sampler/Metropolis.h>

/**
 * @short Random Walk Metropolis Sampler
 *
 * Base class for random walk Metropolis sampler.  This class provides
 * a scale function, which gives a scale by which random walk steps
 * should be multiplied. The scale is adapted to achieve the target
 * acceptance rate using a noisy gradient algorithm whose theoretical
 * properties are unknown.
 */
class RWMetropolis : public Metropolis
{
    const double _prob;
    double _lscale;
    bool  _p_over_target;
    unsigned int _n;
public:
    /**
     * Constructor. Most of the arguments are passed directly to the
     * Metropolis constructor.
     *
     * @param scale Initial scale for the random walk updates.
     * @param prob  Target acceptance probability. The default value seems
     *              to be a fairly robust optimal value.
     */
    RWMetropolis(std::vector<StochasticNode *> const &nodes,
                 Graph const &graph, 
		 unsigned int chain, double const *value, unsigned int length,
		 double scale, double prob = 0.234);
    ~RWMetropolis();
    /**
     * Returns the current scale for the random walk updates.
     */
    double scale() const;
    /**
     * Modifies the scale to achieve the target acceptance probability.
     * Rescaling depends only on the acceptance probability p; the accept
     * parameter is ignored.
     */
    void rescale(double p, bool accept);
    void update(RNG *rng);
};

#endif /* RW_METROPOLIS_H_ */

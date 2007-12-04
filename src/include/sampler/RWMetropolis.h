#ifndef RW_METROPOLIS_H_
#define RW_METROPOLIS_H_

#include <sampler/Metropolis.h>

/**
 * @short Random Walk Metropolis-Hastings update method
 *
 * This class provides a scale function, which gives a scale by which
 * random walk steps should be multiplied. The scale is adapted to 
 * achieve the target acceptance rate using a noisy gradient algorithm.
 */
class RWMetropolis : public Metropolis
{
    const double _prob;
    double _lstep;
    bool  _p_over_target;
    unsigned int _n;
    double _pmean;
    unsigned int _niter;
public:
    /**
     * Constructs a random walk Metropolis sampler. 
     *
     * @param scale Initial scale for the random walk updates.
     *
     * @param prob Target acceptance probability. The default seems to
     *              be a fairly robust optimal value.
     */
    RWMetropolis(std::vector<StochasticNode*> const &nodes, double scale, 
		 double prob = 0.234);
    ~RWMetropolis();
    /**
     * Returns the current step size for the random walk updates.
     */
    double step() const;
    /**
     * Modifies the scale to achieve the target acceptance probability.
     *
     * @param p acceptance probability at current update
     */
    void rescale(double p);
    void update(RNG *rng);
    bool checkAdaptation() const;

};

#endif /* RW_METROPOLIS_H_ */

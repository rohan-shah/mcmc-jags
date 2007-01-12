#
/** 
 * 
 */
class Metropolis : public Sampler
{
    const unsigned int _chain;
    const unsigned int _length;
    const double _prob;
    unsigned int iter;
    double scale;
    double *value;
    double *last_value;
public:
    /**
     * Constructs a Metropolis Hastings sampler
     * 
     * @param nodes Vector of Stochastic Nodes to be sampled
     * @param graph Graph within which sampling is to take place
     * @param chain Chain number that the sampler will update
     * @param value Pointer to the beginning of an array of initial values.
     * @param length Length of initial value array
     */
    Metropolis(std::vector<StochasticNode *> const &nodes, 
	       Graph const &graph, unsigned int chain,
	       double const *value, unsigned int length,
	       double prob);
    ~Metropolis();
    /**
     * Proposes a move to a new value. This function calls setValue,
     * and records the current value of the 
     * update method instead of setValue.
     *
     * @param value Pointer to the beginning of an array of values
     * @param length Length of value array
     */
    void propose(double const *value, unsigned int length);
    /**
     * Accept current value with probabilty p. If the current value is
     * not accepted, the sampler reverts to the value at the last
     * successful call to accept
     *
     * @param rng Random number generator.
     * @param p Probability of accepting the current value.
     *
     * @returns success indicator
     */
    bool accept(RNG *rng, double p);
     /**
     * Sets the value of the sampler, and additionally updates all the
     * deterministic descendants.
     */
    virtual void setValue(double const *value, unsigned int length) = 0;
    /**
     * Returns the number of iterations of the sampler. Initially zero,
     * each call to accept updates the iteration count by 1.
     */
    unsigned int iter() const;
    /**
     * Returns the current value of the chain
     */
    double const *value() const;
};

#endif /* METROPOLIS_H_ */

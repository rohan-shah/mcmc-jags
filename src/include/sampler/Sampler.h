#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <vector>
#include <string>
#include <set>

class RNG;
class StochasticNode;
class GraphView;

/**
 * @short Updates a set of stochastic nodes
 *
 * A sampler updates a set of stochastic nodes.  
 */
class Sampler {
    GraphView *_gv;
public:
    /**
     * Constructor
     *
     * @param gv GraphView providing a view of the sampling graph for
     * the sampler. The Sampler takes ownership of the GraphView and
     * deletes it when the destructor is called.
     */
    Sampler(GraphView *gv);
    virtual ~Sampler();
    /**
     * Returns the vector of stochastic nodes sampled by the Sampler
     */
    std::vector<StochasticNode*> const &nodes() const;
    /**
     * Every sampler must update the vector of nodes and its immediate
     * deterministic descendants using the update function.
     *
     * @param rng vector of Pseudo-random number generator functions.
     */
    virtual void update(std::vector<RNG *> const &rng) = 0;
    /**
     * When a sampler is constructed, it may be in adaptive mode, which
     * allows it to adapt its behaviour for increased
     * efficiency. However, a sampler in adaptive mode may not converge
     * to the correct target distribution. This function turns off
     * adaptive mode, so that valid samples can be collected from the
     * sampler.
     */
    virtual void adaptOff() = 0;
    /*
     * The adaptOff function may be called at any time. Premature
     * ending of adaptive mode may result in an extremely inefficient
     * sampler.  Therefore the checkAdaptation function implements an
     * efficiency test that returns true if it is safe to call the
     * adaptOff function.  Samplers that have no adaptive mode should
     * simply return true.
     */
    virtual bool checkAdaptation() const = 0;
    /**
     * Indicates whether the sampler has an adaptive mode.
     */
    virtual bool isAdaptive() const = 0;
    /**
     * Returns a name for the sampler which should describe the method
     * it uses to update the nodes.
     */
    virtual std::string name() const = 0;
};

#endif /* SAMPLER_H_ */

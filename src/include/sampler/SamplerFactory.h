#ifndef SAMPLER_FACTORY_H_
#define SAMPLER_FACTORY_H_

#include <vector>
#include <set>
#include <string>

class Sampler;
class StochasticNode;
class Graph;

/**
 * @short Factory for Sampler objects
 */
class SamplerFactory
{
public:
    virtual ~SamplerFactory();
    /**
     * Finds nodes in the set of stochastic nodes that can be sampled
     * within the given graph, and returns a newly allocated sampler
     * for them.  If no sampler can be created, a NULL pointer is
     * returned.
     */
    virtual Sampler* makeSampler(std::set<StochasticNode*> const &nodes, 
				 Graph const &graph) const = 0;
     /**
      * Returns the name of the sampler factory
      */
    virtual std::string const &name() const = 0;

};

#endif /* SAMPLER_FACTORY_H_ */

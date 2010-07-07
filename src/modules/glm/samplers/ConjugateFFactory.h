#ifndef CONJUGATE_F_FACTORY_H_
#define CONJUGATE_F_FACTORY_H_

#include <sampler/SingletonFactory.h>
#include <string>

/**
 * @short Factory object for conjugate samplers
 */
class ConjugateFFactory : public SingletonFactory
{
public:
    /**
     * Provides a simple interface to the canSample member functions of
     * the various conjugate samplers.  The distribution of snode is found
     * and, if it has an associated conjugate sampler, the appropriate
     * canSample member function is dispatched
     */
    bool canSample(StochasticNode *snode, Graph const &graph) const;
    /**
     * Creates a ConjugateFSampler for a stochastic node. 
     */
    Sampler *makeSampler(StochasticNode *snode, Graph const &graph) const;
    std::string name() const;
};

#endif /* CONJUGATE_F_FACTORY_H_ */

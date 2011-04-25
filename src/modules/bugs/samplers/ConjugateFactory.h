#ifndef CONJUGATE_FACTORY_H_
#define CONJUGATE_FACTORY_H_

#include <sampler/SingletonFactory.h>

namespace bugs {

/**
 * @short Factory object for conjugate samplers
 */
class ConjugateFactory : public SingletonFactory
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
     * Creates a ConjugateSampler for a stochastic node. 
     */
    Sampler *makeSampler(StochasticNode *snode, Graph const &graph) const;
    std::string name() const;
};

}

#endif /* CONJUGATE_FACTORY_H_ */

#ifndef CONJUGATE_FACTORY_H_
#define CONJUGATE_FACTORY_H_

#include "ConjugateSampler.h"

#include <sampler/SingletonFactory.h>
#include <map>
#include <string>

/**
 * @short Factory object for conjugate samplers
 */
class ConjugateFactory : public SingletonFactory
{
    std::string _name;
    std::map<std::string, bool (*)(StochasticNode *,
				   Graph const &)> _func_table;
public:
    ConjugateFactory();
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
    std::string const &name() const;
};

#endif /* CONJUGATE_FACTORY_H_ */

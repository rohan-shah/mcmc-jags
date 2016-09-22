#ifndef RW1_FACTORY_H_
#define RW1_FACTORY_H_

#include "RW1.h"

#include <sampler/SingletonFactory.h>

namespace jags {
    namespace bugs {

	/**
	 * @short Factory object for RW1 sampler
	 */
	class RW1Factory : public SingletonFactory
	{
	  public:
	    bool canSample(StochasticNode *snode, Graph const &graph) const;
	    Sampler *makeSampler(StochasticNode *, Graph const &) const;
	    std::string name() const;
	};
    }
}

#endif /* RW1_FACTORY_H_ */

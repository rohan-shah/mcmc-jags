#ifndef CENSORED_FACTORY_H_
#define CENSORED_FACTORY_H_

#include <sampler/SingletonFactory.h>

namespace jags {

    class Graph;

    namespace bugs {

	/**
	 * @short Factory object for censored samplers
	 */
	class CensoredFactory : public SingletonFactory
	{
	public:
	    bool canSample(StochasticNode *, Graph const &) const;
	    Sampler *makeSampler(StochasticNode *, Graph const &) const;
	    std::string name() const;
	};

    }
}

#endif /* CENSORED_FACTORY_H_ */

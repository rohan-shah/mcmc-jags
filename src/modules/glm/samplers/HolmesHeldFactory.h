#ifndef HOLMES_HELD_FACTORY_H_
#define HOLMES_HELD_FACTORY_H_

#include "BinaryFactory.h"

namespace jags {
namespace glm {

    /**
     * @short Factory object for the Holmes-Held sampling method
     */
    class HolmesHeldFactory : public BinaryFactory
    {
    public:
	HolmesHeldFactory();
	/**
	 * Returns a newly allocated object of class HolmesHeld for
	 * sampling binary GLMs with probit or logistic link.
	 */
	GLMMethod *newBinary(GraphView const *view, 
			     std::vector<SingletonGraphView const *> const &sub_views,
			     std::vector<Outcome *> const &outcomes,
			     unsigned int chain) const;
    };

}}

#endif /* HOLMES_HELD_FACTORY_H_ */

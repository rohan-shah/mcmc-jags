#ifndef HOLMES_HELD_FACTORY_H_
#define HOLMES_HELD_FACTORY_H_

#include "BinaryFactory.h"

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
	BinaryGLM *newBinary(GraphView const *view, 
			     std::vector<GraphView const *> const &sub_views,
			     unsigned int chain) const;
	/**
	 * Returns true. The Holmes-Held update method requires a
	 * fixed outcome because the auxiliary variables are updated
	 * by a reversible transition from their current values.
	 */
	bool fixedOutcome() const;
	/**
	 * Returns true. The Holmes-Held update method requires a
	 * fixed design matrix
	 */
	bool fixedDesign() const;
    };

}

#endif /* HOLMES_HELD_FACTORY_H_ */

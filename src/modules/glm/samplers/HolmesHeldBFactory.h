#ifndef HOLMES_HELD_B_FACTORY_H_
#define HOLMES_HELD_B_FACTORY_H_

#include "BinaryFactory.h"
#include "BinaryGLM.h"

namespace glm {

    /**
     * @shortFactory object for conjugate linear model sampler
     */
    class HolmesHeldBFactory : public BinaryFactory
    {
    public:
	HolmesHeldBFactory();
	BinaryGLM *newBinary(GraphView const *view, 
			     std::vector<GraphView const *> const &sub_views,
			     unsigned int chain) const;
    };

}

#endif /* HOLMES_HELD_B_FACTORY_H_ */

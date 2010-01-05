#ifndef HOLMES_HELD_H_
#define HOLMES_HELD_H_

#include "BinaryGLM.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class HolmesHeld : public BinaryGLM {
    public:
	HolmesHeld(GraphView const *view, 
		   std::vector<GraphView const *> const &sub_views,
		   unsigned int chain);
	void updateAuxiliary(double *b, csn *N, RNG *rng);
	std::string name() const;
	void update(RNG *rng);
    };
    
}

#endif /* HOLMES_HELD_H_ */

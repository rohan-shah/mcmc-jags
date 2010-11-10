#ifndef HOLMES_HELD_B_H_
#define HOLMES_HELD_B_H_

#include "BinaryGLM.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class HolmesHeldB : public BinaryGLM {
	std::vector<double> _z1;
	bool _init;
    public:
	HolmesHeldB(GraphView const *view, 
		    std::vector<GraphView const *> const &sub_views,
		    unsigned int chain);
	void updateAuxiliary(double *b, csn *N, RNG *rng);
	std::string name() const;
	bool update(RNG *rng);
    };
    
}

#endif /* HOLMES_HELD_B_H_ */

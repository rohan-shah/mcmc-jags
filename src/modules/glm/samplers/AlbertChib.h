#ifndef ALBERT_CHIB_H_
#define ALBERT_CHIB_H_

#include "BinaryGLM.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class AlbertChib : public BinaryGLM {
	bool _gibbs;
    public:
	AlbertChib(Updater const *updater, 
		   std::vector<Updater const *> const &sub_updaters,
		   unsigned int chain, bool gibbs);
	std::string name() const;
	void update(RNG *rng);
    };
    
}

#endif /* ALBERT_CHIB_H_ */

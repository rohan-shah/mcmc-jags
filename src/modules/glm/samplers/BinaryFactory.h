#ifndef BINARY_FACTORY_H_
#define BINARY_FACTORY_H_

#include "GLMFactory.h"

namespace glm {

    class BinaryGLM;

    class BinaryFactory : public GLMFactory
    {
    public:
	BinaryFactory(std::string const &name);
	bool checkOutcome(StochasticNode const *snode,
			  LinkNode const *lnode) const;
	GLMMethod *newMethod(Updater const *updater,
			     std::vector<Updater const *> const &sub_updaters,
			     unsigned int chain) const;
	virtual BinaryGLM *newBinary(Updater const *updater,
				     std::vector<Updater const *> const 
				     &sub_updaters,
				     unsigned int chain) const = 0;
    };

}

#endif /* BINARY_FACTORY_H_ */

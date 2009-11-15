#include <config.h>

#include "AlbertChibFactory.h"
#include "AlbertChib.h"

using std::vector;

namespace glm {

    AlbertChibFactory::AlbertChibFactory()
	: BinaryFactory("Albert-Chib")
    {}

    BinaryGLM *
    AlbertChibFactory::newBinary(Updater const *updater,
				 vector<Updater const *> const &sub_updaters,
				 unsigned int chain) const
    {
	return new AlbertChib(updater, sub_updaters, chain, false);
    }

    bool AlbertChibFactory::trunc() const
    {
	return false;
    }

}

#include <config.h>

#include "AlbertChibGibbsFactory.h"
#include "AlbertChib.h"

using std::vector;

namespace glm {

    AlbertChibGibbsFactory::AlbertChibGibbsFactory()
	: BinaryFactory("Albert-Chib-Gibbs")
    {}

    BinaryGLM *
    AlbertChibGibbsFactory::newBinary(Updater const *updater,
				 vector<Updater const *> const &sub_updaters,
				 unsigned int chain) const
    {
	return new AlbertChib(updater, sub_updaters, chain, true);
    }

    bool AlbertChibGibbsFactory::trunc() const
    {
	return true;
    }

}

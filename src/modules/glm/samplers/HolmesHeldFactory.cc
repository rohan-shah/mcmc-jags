#include <config.h>

#include "HolmesHeldFactory.h"
#include "HolmesHeld.h"

using std::vector;

namespace glm {

    HolmesHeldFactory::HolmesHeldFactory()
	: BinaryFactory("Holmes-Held")
    {}

    BinaryGLM *
    HolmesHeldFactory::newBinary(Updater const *updater,
				 vector<Updater const *> const &sub_updaters,
				 unsigned int chain) const
    {
	return new HolmesHeld(updater, sub_updaters, chain);
    }

    bool HolmesHeldFactory::trunc() const
    {
	return false;
    }

}

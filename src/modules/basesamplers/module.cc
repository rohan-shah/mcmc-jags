#include <Module.h>

#include "SliceFactory.h"
#include "FiniteFactory.h"

using std::vector;

namespace basesamplers {

class BaseSamplerModule : public Module {
public:
    BaseSamplerModule();
    ~BaseSamplerModule();
};

BaseSamplerModule::BaseSamplerModule() {

    insert(new SliceFactory);
    insert(new FiniteFactory);
}

BaseSamplerModule::~BaseSamplerModule() {

    vector<SamplerFactory*> const &svec = samplerFactories();
    for (unsigned int i = 0; i < svec.size(); ++i) {
	delete svec[i];
    }
}

}

basesamplers::BaseSamplerModule _base_sampler_module;

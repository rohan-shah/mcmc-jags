#include <Module.h>
#include <distribution/DistTab.h>

#include <distributions/DNormMix.h>
#include <samplers/MixSamplerFactory.h>

using std::vector;

class MIXModule : public Module {
public:
    MIXModule();
    ~MIXModule();
};

MIXModule::MIXModule() {

    //Load distributions
    insert(new DNormMix);
    insert(new MixSamplerFactory);
}

MIXModule::~MIXModule() {

    vector<Distribution*> const &dvec = distributions();
    for (unsigned int i = 0; i < dvec.size(); ++i) {
	delete dvec[i];
    }

    vector<SamplerFactory*> const &svec = samplerFactories();
    for (unsigned int i = 0; i < svec.size(); ++i) {
        delete svec[i];
    }
}

MIXModule _mix_module;

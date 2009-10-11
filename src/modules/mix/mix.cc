#include <Module.h>

#include <distributions/DNormMix.h>
#include <samplers/MixSamplerFactory.h>

using std::vector;

namespace mix {

    class MIXModule : public Module {
    public:
	MIXModule();
	~MIXModule();
    };

    MIXModule::MIXModule() 
	: Module("mix") {

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

}

mix::MIXModule _mix_module;

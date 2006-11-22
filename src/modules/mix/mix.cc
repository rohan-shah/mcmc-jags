#include <Module.h>
#include <distribution/DistTab.h>

#include <distributions/DNormMix.h>

using std::vector;

class MIXModule : public Module {
public:
    MIXModule();
    ~MIXModule();
};

MIXModule::MIXModule() {

    //Load distributions
    insert(new DNormMix);
}

MIXModule::~MIXModule() {

    vector<Distribution*> const &dvec = distributions();
    for (unsigned int i = 0; i < dvec.size(); ++i) {
	delete dvec[i];
    }

}

MIXModule _mix_module;

#include <module/Module.h>

#include "DCut.h"
#include "CutSamplerFactory.h"

using std::vector;
using namespace cut;

class CutModule : public Module {
public:
    CutModule();
    ~CutModule();
};

CutModule::CutModule() 
    : Module("cut")
{

    //Load distributions
    insert(new DCut);

    //Load sampler factories
    insert(new CutSamplerFactory);
}

CutModule::~CutModule() {

    vector<Distribution*> const &dvec = distributions();
    for (unsigned int i = 0; i < dvec.size(); ++i) {
	delete dvec[i];
    }

    vector<SamplerFactory*> const &svec = samplerFactories();
    for (unsigned int i = 0; i < svec.size(); ++i) {
	delete svec[i];
    }
}

CutModule _cut_module;

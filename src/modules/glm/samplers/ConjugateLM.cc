#include <config.h>

#include "ConjugateLM.h"
#include "GLMSampler.h"
#include <graph/StochasticNode.h>
#include <sampler/Updater.h>

using std::vector;
using std::string;

namespace glm {

    ConjugateLM::ConjugateLM(Updater const *updater)
	: GLMMethod(updater), _updater(updater)
    {
    }

    double ConjugateLM::getMean(unsigned int i, unsigned int chain) const
    {
	return _updater->stochasticChildren()[i]->parents()[0]->value(chain)[0];
    }
    
    double 
    ConjugateLM::getPrecision(unsigned int i, unsigned int chain) const 
    {
	return _updater->stochasticChildren()[i]->parents()[1]->value(chain)[0];
    }

    double ConjugateLM::getValue(unsigned int i, unsigned int chain) const 
    {
	return _updater->stochasticChildren()[i]->value(chain)[0];
    }
    
    string ConjugateLM::name() const
    {
	return "ConjugateLM";
    }
    
}

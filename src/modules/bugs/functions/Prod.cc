#include <config.h>
#include <util/logical.h>
#include "Prod.h"

using std::vector;

namespace jags {
namespace bugs {

    Prod::Prod () : ScalarVectorFunction("prod", 0)
    {
    }

    double Prod::evaluate(vector <double const *> const &args,
			  vector<unsigned int> const &lengths) const
    {
	double value = 1;
	for (unsigned int j = 0; j < args.size(); ++j) {
	    for (unsigned int i = 0; i < lengths[j]; ++i) {
		value *= args[j][i];
	    }
	}
	return value;
    }

    bool Prod::checkParameterLength (vector<unsigned int> const &args) const
    {
	return true;
    }

    bool Prod::isDiscreteValued(vector<bool> const &mask) const
    {
	return allTrue(mask);
    }

}}

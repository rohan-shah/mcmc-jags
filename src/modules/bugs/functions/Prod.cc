#include <config.h>
#include <util/logical.h>
#include "Prod.h"

using std::vector;

namespace bugs {

    Prod::Prod () : VectorFunction("prod", 1)
    {
    }

    void Prod::evaluate(double *x, vector <double const *> const &args,
			vector<unsigned int> const &lengths) const
    {
	double value = args[0][0];
	for (unsigned int i = 1; i < lengths[0]; ++i) {
	    value *= args[0][i];
	}
	*x = value;
    }

    bool Prod::checkParameterLength (vector<unsigned int> const &args) const
    {
	return true;
    }

    bool Prod::isDiscreteValued(vector<bool> const &mask) const
    {
	return allTrue(mask);
    }

}

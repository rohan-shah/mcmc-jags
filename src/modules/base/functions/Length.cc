#include <config.h>
#include <sarray/util.h>
#include "Length.h"

using std::vector;

namespace base {

    Length::Length () : Function("length", 1)
    {
    }

    void Length::evaluate(double *x, vector <double const *> const &args,
			  vector<unsigned int> const &lengths,
			  vector<vector<unsigned int> > const &dims) const
    {
	*x = lengths[0];
    }

    bool Length::isDiscreteValued(vector<bool> const &mask) const
    {
	return true;
    }

    bool Length::checkParameterDim(vector<vector<unsigned int> > const &dims) 
	const
    {
	return true;
    }
}

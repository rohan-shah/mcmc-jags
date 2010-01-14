#include <config.h>
#include <util/logical.h>
#include "Max.h"

#include <algorithm>

using std::vector;
using std::max_element;
using std::max;

namespace bugs {

    Max::Max ()
	: VectorFunction("max", 0)
    {
    }

    void Max::evaluate(double *value,  vector<double const *> const &args,
		       vector<unsigned int> const &lengths) const
    {
	double ans = *max_element(args[0], args[0] + lengths[0]);
	for (unsigned int i = 1; i < args.size(); ++i) {
	    double maxi = *max_element(args[i], args[i] + lengths[i]);
	    ans = max(ans, maxi);
	}
	*value = ans;
    }

    bool Max::checkParameterLength (vector<unsigned int> const &lengths) const
    {
	return true;
    }

    bool Max::isDiscreteValued(vector<bool> const &mask) const
    {
	return allTrue(mask);
    }

}

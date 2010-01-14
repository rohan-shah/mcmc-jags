#include <config.h>
#include <util/logical.h>
#include "Min.h"

#include <algorithm>

using std::min_element;
using std::min;
using std::vector;

namespace bugs {

    Min::Min ()
	: VectorFunction ("min", 0)
    {
    }

    void Min::evaluate(double *value,  vector<double const *> const &args,
		       vector<unsigned int> const &lengths) const
    {
	double ans = *min_element(args[0], args[0] + lengths[0]);
	for (unsigned int i = 1; i < args.size(); ++i) {
	    double mini = *min_element(args[i], args[i] + lengths[i]);
	    ans = min(ans, mini);
	}
	*value = ans;
    }

    bool Min::checkParameterLength (vector<unsigned int> const &args) const
    {
	return true;
    }

    bool Min::isDiscreteValued(vector<bool> const &mask) const
    {
	return allTrue(mask);
    }

}

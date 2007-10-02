#include <config.h>
#include <sarray/util.h>
#include "Min.h"

#include <algorithm>

using std::min_element;
using std::min;
using std::vector;

namespace bugs {

    Min::Min ()
	: Function ("min", 0)
    {
    }

    void Min::evaluate(double *value,  vector<double const *> const &args,
		       vector<unsigned int> const &lengths,
		       vector<vector<unsigned int> > const &dims) const
    {
	double ans = *min_element(args[0], args[0] + lengths[0]);
	for (unsigned int i = 1; i < args.size(); ++i) {
	    double mini = *min_element(args[i], args[i] + lengths[i]);
	    ans = min(ans, mini);
	}
	*value = ans;
    }

    bool Min::checkParameterDim (vector<vector<unsigned int> > const &dims) const
    {
	return true;
    }

    bool Min::isDiscreteValued(vector<bool> const &mask) const
    {
	return allTrue(mask);
    }

}

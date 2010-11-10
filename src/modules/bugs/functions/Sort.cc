#include <config.h>
#include "Sort.h"

#include <algorithm>

#include <util/dim.h>

using std::vector;
using std::sort;

namespace bugs {

    Sort::Sort ()
	: VectorFunction ("sort", 1)
    {
    }

    void Sort::evaluate (double *value, vector <double const *> const &args,
			 vector<unsigned int> const &lengths) const
    {
	for (unsigned int i = 0; i < lengths[0]; ++i) {
	    value[i] = args[0][i];
	}
	sort(value, value + lengths[0]);
    }

    unsigned int Sort::length (vector<unsigned int> const &parlengths) const
    {
	return parlengths[0];
    }

    bool Sort::isDiscreteValued(vector<bool> const &mask) const
    {
#ifdef _RWSTD_NO_CLASS_PARTIAL_SPEC
//workaround for Solaris libCstd
        int n = 0; 
        count(mask.begin(), mask.end(), false, n);
        return n == 0;
#else
	return count(mask.begin(), mask.end(), false) == 0;
#endif
    }

}

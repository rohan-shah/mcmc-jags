#include <config.h>
#include <util/dim.h>
#include "Rank.h"

#include <algorithm>

using std::vector;
using std::stable_sort;

static bool lt_doubleptr (double const *arg1, double const *arg2) {
  return *arg1 < *arg2;
}

namespace bugs {

    Rank::Rank ()
	: VectorFunction ("rank", 1)
    {
    }

    void Rank::evaluate(double *value, vector<double const *> const &args,
			vector<unsigned int> const &lengths) const
    {
	int N = lengths[0];

	//Create a vector of pointers to the elements of arg and sort it
	double const **argptrs = new double const *[N];
	for (int i = 0; i < N; ++i) {
	    argptrs[i] = args[0] + i;
	}
	stable_sort(argptrs, argptrs + N, lt_doubleptr);

	//Ranks can be inferred from the sorted vector of pointers
	for (int i = 0; i < N; ++i) {
	    value[argptrs[i] - args[0]] = i + 1;
	}
	delete [] argptrs;
    }

    unsigned int Rank::length (vector<unsigned int> const &lengths) const
    {
	return lengths[0];
    }

    bool Rank::isDiscreteValued(vector<bool> const &mask) const
    {
	return true;
    }

}

#include <config.h>
#include <function/ScalarFunc.h>
#include <sarray/util.h>

#include <algorithm>

using std::vector;
using std::string;
using std::find_if;

ScalarFunc::ScalarFunc (string const &name, unsigned int npar)
  : Function (name, npar)
{
}

void
ScalarFunc::evaluate (double *value, vector<double const *> const &args,
		      vector<unsigned int> const &lengths,
                      vector<vector<unsigned int> > const &dims) const
{
    unsigned int N = 1;
    for (unsigned int i = 0; i < lengths.size(); ++i) {
	if (lengths[i] != 1) {
	    N = lengths[i];
	    break;
	}
    }

    value[0] = eval(args);
    if (N > 1) {
	vector<double const *> vecargs(args);
	for (unsigned int j = 1; j < N; ++j) {
	    for (unsigned int i = 0; i < lengths.size(); ++i) {
		vecargs[i] += (lengths[i] != 1);
	    }
	    value[j] = eval(vecargs);
	}
    }
}

vector<unsigned int> 
ScalarFunc::dim(vector<vector<unsigned int> > const &args) const
{
    for (unsigned int i = 0; i < args.size(); ++i) {
	if (!isScalar(args[i])) {
	    return args[i];
	}
    }
    return vector<unsigned int>(1,1);
}

bool
ScalarFunc::checkParameterDim(vector<vector<unsigned int> > const &dims) const
{
    vector<unsigned int> d = dims[0];
    for (unsigned int i = 1; i < dims.size(); ++i) {
	if (!isScalar(dims[i])) {
	    if (isScalar(d)) {
		d = dims[i];
	    }
	    else if (d != dims[i]) {
		return false;
	    }
	}
    }
    return true;
}

bool 
ScalarFunc::checkParameterValue(vector<double const *> const &args,
				vector<unsigned int> const &lengths,
				vector<vector<unsigned int> > const &dims)
    const
{
    unsigned int N = 1;
    for (unsigned int i = 0; i < lengths.size(); ++i) {
	if (lengths[i] != 1) {
	    N = lengths[i];
	    break;
	}
    }

    if (!checkParameterValue(args)) {
	return false;
    }
    if (N > 1) {
	vector<double const *> vecargs(args);
	for (unsigned int j = 1; j < N; ++j) {
	    for (unsigned int i = 0; i < lengths.size(); ++i) {
		vecargs[i] += (lengths[i] != 1);
	    }
	    if (!checkParameterValue(vecargs)) {
		return false;
	    }
	}
    }

    return true;
}

bool ScalarFunc::checkParameterValue(vector<double const *> const &args) const
{
    return true;
}

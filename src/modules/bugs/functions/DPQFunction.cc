#include <config.h>
#include "DPQFunction.h"

#include <distribution/DistScalarRmath.h>

using std::vector;
using std::string;

namespace bugs {

    DPQFunction::DPQFunction(string const &name, DistScalarRmath *dist) 
	: ScalarFunction(name, dist->npar() + 1), _dist(dist),
	  _dims(dist->npar(),vector<unsigned int>(1,1))
    {}
    
    DPQFunction::~DPQFunction()
    {
	delete _dist;
    }

    DistScalarRmath const *DPQFunction::dist() const
    {
	return _dist;
    }

    bool DPQFunction::checkArgs(vector<double const *> const &args) const
    {
	vector<double const *> param(args.size() - 1);
	for (unsigned int i = 1; i < args.size(); ++i) {
	    param[i-1] = args[i];
	}
	
	return _dist->checkParameterValue(param, _dims);
    }
}

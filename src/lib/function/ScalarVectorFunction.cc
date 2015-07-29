#include <config.h>
#include <function/ScalarVectorFunction.h>

using std::vector;
using std::string;

namespace jags {

    ScalarVectorFunction::ScalarVectorFunction (string const &name, 
						unsigned int npar)
	: VectorFunction(name, npar)
    {
    }

    void 
    ScalarVectorFunction::evaluate(double *value, 
				   vector <double const *> const &args,
				   vector <unsigned int> const &lengths) const
    {
	*value = scalarEval(args, lengths);
    }

    unsigned int 
    ScalarVectorFunction::length(vector <unsigned int> const &lengths,
				 vector <double const *> const &values) const
    {
	return 1;
    }

}

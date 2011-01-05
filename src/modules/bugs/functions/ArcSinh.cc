#include <config.h>
#include "ArcSinh.h"

#include <cmath>

using std::vector;
using std::log;
using std::sqrt;
using std::string;

/* We don't use the cmath function asinh because it is defined in C99
   and is not necessarily available in C++, which uses the C90
   standard. This creates portability problems as we cannot guarantee
   that asinh will be in the std namespace or not.x  

   The same issue affects ArcCosh (acosh) and ArcTanh (atanh).
*/

namespace bugs {

    ArcSinh::ArcSinh ()
	: ScalarFunction ("arcsinh", 1)
    {
    }

    string ArcSinh::alias() const
    {
	return "asinh";
    }

    double ArcSinh::evaluate(vector<double const *> const &args) const
    {
	double x = *args[0];
	return log(x + sqrt(1 + x*x));
    }

}

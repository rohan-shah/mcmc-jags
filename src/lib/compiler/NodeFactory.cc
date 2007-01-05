#include <compiler/NodeFactory.h>
#include <graph/Node.h>
#include <sarray/nainf.h>

#include <cfloat>
#include <cmath>

static const double tol = 16 * DBL_EPSILON;

bool equal(double arg1, double arg2)
{
  if (arg1 == JAGS_NA || arg2 == JAGS_NA)
    return false;
  else 
    return !(arg1 + tol < arg2 || arg2 + tol < arg1);
}

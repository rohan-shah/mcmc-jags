#include "Bounds.h"
#include <distribution/Distribution.h>
#include <vector>

using std::vector;

double const *
lowerBound(Distribution const *dist, vector<double const *> const &args) 
{
  return dist->canBound() ? args[dist->npar()] : 0;
}

double const *
upperBound(Distribution const *dist, vector<double const *> const &args) 
{
  return dist->canBound() ? args[dist->npar() + 1] : 0;
}


#include "Bounds.h"
#include <distribution/Distribution.h>
#include <vector>

using std::vector;

SArray const *
lowerBound(Distribution const *dist, vector<SArray const *> const &args) 
{
  return dist->canBound() ? args[dist->npar()] : 0;
}

SArray const *
upperBound(Distribution const *dist, vector<SArray const *> const &args) 
{
  return dist->canBound() ? args[dist->npar() + 1] : 0;
}


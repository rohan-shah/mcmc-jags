#include <config.h>
#include "Cos.h"

#include <cmath>

using std::vector;
using std::cos;

Cos::Cos ()
  : ScalarFunc ("cos", 1)
{
}

double Cos::eval(vector<double const *> const &args) const
{
  return cos(*args[0]);
}

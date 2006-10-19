#include <config.h>
#include "Log.h"

#include <cmath>

using std::vector;
using std::log;


Log::Log ()
  : ScalarFunc ("log", 1)
{
}

double Log::eval(vector<double const *> const &args) const
{
  return log(*args[0]);
}

bool Log::checkParameterValue(vector<double const *> const &args) const
{
  return *args[0] >= 0;
}

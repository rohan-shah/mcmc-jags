#include <config.h>
#include "LogGam.h"

#include <JRmath.h>

using std::vector;

LogGam::LogGam ()
  : ScalarFunc ("loggam", 1)
{
}

double LogGam::eval(vector<double const *> const &args) const
{
  return lgammafn (*args[0]);
}

bool LogGam::checkParameterValue(vector<double const *> const &args) const
{
  return *args[0] > 0;
}

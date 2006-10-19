#include <config.h>
#include "LogFact.h"

#include <JRmath.h>

using std::vector;

LogFact::LogFact ()
  : ScalarFunc ("logfact", 1)
{
}

double LogFact::eval(vector<double const *> const &args) const
{
  return lgammafn(*args[0] + 1);
}

bool LogFact::checkParameterValue(vector<double const *> const &args) const
{
  return *args[0] > -1;
}

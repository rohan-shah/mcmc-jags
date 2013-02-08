#include <config.h>
#include <distribution/DistError.h>
#include <distribution/Distribution.h>

using std::string;

namespace jags {

DistError::DistError(Distribution const *edist, string const &emsg)
    : runtime_error(emsg + " in distribution " + edist->name())
{
}

} //namespace jags

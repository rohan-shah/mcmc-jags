#include <config.h>
#include <distribution/DistError.h>
#include <distribution/Distribution.h>

using std::string;

DistError::DistError(Distribution const *edist, string const &emsg)
    : runtime_error(emsg + " in distribution " + edist->name())
{
}

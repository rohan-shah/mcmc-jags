#include <config.h>
#include <graph/NodeError.h>

using std::string;

namespace jags {

NodeError::NodeError(Node const *enode, string const &emsg)
  : runtime_error(emsg), node(enode)
{
}

} //namespace jags

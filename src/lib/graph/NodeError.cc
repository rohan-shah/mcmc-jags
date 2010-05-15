#include <config.h>
#include <graph/NodeError.h>

using std::string;

NodeError::NodeError(Node const *enode, string const &emsg)
  : runtime_error(emsg), node(enode)
{
}

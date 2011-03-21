#include <config.h>

#include <module/ModuleError.h>
#include <graph/NodeError.h>

#include <stdexcept>

using std::string;
using std::runtime_error;

static string _error_message;
static Node const *_error_node;

void setModuleError(string const &message, Node const *node)
{
    _error_message = message;
    _error_node = node;
}

void throwModuleError()
{
    if (!_error_message.empty()) {

	string emsg = _error_message; _error_message.clear();
	Node const *enode = _error_node; enode = 0;
	
	if (enode) {
	    throw NodeError(enode, emsg);
	}
	else {
	    throw runtime_error(emsg);
	}
    }

}

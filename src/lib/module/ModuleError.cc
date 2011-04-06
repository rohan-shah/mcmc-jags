#include <config.h>

#include <module/ModuleError.h>
#include <graph/NodeError.h>
#include <distribution/DistError.h>
#include <function/FuncError.h>

using std::string;
using std::runtime_error;
using std::logic_error;

void throwLogicError(string const &message)
{
    throw logic_error(message);
}

void throwRuntimeError(string const &message)
{
    throw runtime_error(message);
}

void throwNodeError(Node const *node, string const &message)
{
    throw NodeError(node, message);
}

void throwDistError(Distribution const *dist, string const &message)
{
    throw DistError(dist, message);
}

void throwFuncError(Function const *func, string const &message)
{
    throw FuncError(func, message);
}

#ifndef INFIX_H_
#define INFIX_H_

#include <function/ScalarFunction.h>

namespace base {

/**
 * Base class for infix operators, providing a deparse method.
 *  
 * Most infix operators are binary, so we also provide a default value
 * of 2 for the number of parameters.
 */
class Infix : public ScalarFunction
{
public:
    Infix(std::string const &name, unsigned int npar=2);
    std::string deparse(std::vector<std::string> const &par) const;
};

}

#endif /* INFIX_H_ */

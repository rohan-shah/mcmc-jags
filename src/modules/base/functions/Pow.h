#ifndef FUNC_POW_H_
#define FUNC_POW_H_

#include "Infix.h"

namespace base {
    
/**
 * @short Power function using infix notation
 * If y <- x^z and x is negative, then z must be an integer
 */
class Pow : public Infix
{
public:
    Pow ();
    double evaluateScalar(std::vector<double const *> const &args) const;
    bool checkScalarValue(std::vector<double const*> const &args) const;
};

}

#endif /* FUNC_POW_H_ */

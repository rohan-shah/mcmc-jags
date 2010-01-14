#ifndef FUNC_OR_H_
#define FUNC_OR_H_

#include "Infix.h"

namespace base {

/**
 * The Or function implements the infix operator "|" in the BUGS 
 * language
 * @short Logical or
 * <pre>
 * y <- a|b
 * y = 1 if a != 0 or  b != 0
 * y = 0 otherwise
 * </pre>
 */
class Or:public Infix
{
public:
    Or ();
    double evaluate(std::vector<double const *> const &args) const;
    bool isDiscreteValued(std::vector<bool> const &mask) const;
};

}

#endif /* FUNC_OR_H_ */

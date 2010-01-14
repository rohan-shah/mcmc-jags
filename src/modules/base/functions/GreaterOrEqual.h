#ifndef FUNC_GREATER_OR_EQUAL_H_
#define FUNC_GREATER_OR_EQUAL_H_

#include "Infix.h"

namespace base {

/**
 * @short Numeric comparison
 * <pre>
 * y <- a >= b
 * y = 1 if a >= b
 *     0 if a < b
 * </pre>
 */
class GreaterOrEqual:public Infix
{
public:
    GreaterOrEqual ();
    double evaluate(std::vector<double const *> const &args) const;
    /** Returns true */
    bool isDiscreteValued(std::vector<bool> const &mask) const;
};

}

#endif /* FUNC_GREATER_OR_EQUAL_H_ */

#ifndef FUNC_LESS_THAN_H_
#define FUNC_LESS_THAN_H_

#include "Infix.h"

namespace base {

/**
 * @short Numeric comparison
 * <pre>
 * y <- a < b
 * y = 1 if a < b
 *     0 if a >= b
 * </pre>
 */
class LessThan:public Infix
{
public:
    LessThan ();
    double evaluate(std::vector<double const *> const &args) const;
    /** Returns true */
    bool isDiscreteValued(std::vector<bool> const &mask) const;
};

}

#endif /* FUNC_LESS_THAN_H_ */

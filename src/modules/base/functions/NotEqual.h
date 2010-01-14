#ifndef FUNC_NOT_EQUAL_H_
#define FUNC_NOT_EQUAL_H_

#include "Infix.h"

namespace base {

/**
 * @short Test for equality using infix notation
 * <pre>
 * y <- a != b
 * y = 0 if a==b
 *     1 if a!=b
 * </pre>
 */
class NotEqual:public Infix
{
public:
    NotEqual ();
    double evaluate(std::vector<double const *> const &args) const;
    /** Returns true */
    bool isDiscreteValued(std::vector<bool> const &mask) const;
};

}

#endif /* FUNC_NOT_EQUAL_H_ */

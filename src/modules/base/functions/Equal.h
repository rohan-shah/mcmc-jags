#ifndef FUNC_EQUAL_H_
#define FUNC_EQUAL_H_

#include "Infix.h"

namespace base {

/**
 * @short Test for equality using infix notation
 * <pre>
 * y <- a == b
 * y = 1 if a==b
 *     0 if a!=b
 * </pre>
 */
class Equal:public Infix
{
public:
    Equal ();
    double evaluate(std::vector<double const *> const &args) const;
    /** Returns true */
    bool isDiscreteValued(std::vector<bool> const &mask) const;
};

}

#endif /* FUNC_EQUAL_H_ */

#ifndef IF_ELSE_H_
#define IF_ELSE_H_

#include <function/ScalarFunction.h>

namespace bugs {

/**
 * The ifelse function returns either a or b depending on whether the 
 * value of x is zero or not
 * 
 * @short Conditional selection 
 * <pre>
 * y <- ifelse(x,a,b)
 * y = a if x != 0 
 * y = b if x == 0
 * </pre>
 */
class IfElse : public ScalarFunction
{
public:
    IfElse ();
    double evaluate(std::vector<double const *> const &args) const;
    bool isDiscreteValued(std::vector<bool> const &mask) const;
};

}

#endif /* IF_ELSE_H_ */

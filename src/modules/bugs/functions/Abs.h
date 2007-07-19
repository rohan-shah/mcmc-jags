#ifndef FUNC_ABS_H_
#define FUNC_ABS_H_

#include <function/ScalarFunc.h>

/**
* @short Absolute value
 * <pre>
 * y <- abs(x)
 * y = x; x >= 0
 * y = -1; x < 0
 * </pre>
 */
class Abs: public ScalarFunc
{
public:
    Abs ();
    double eval(std::vector<double const *> const &args) const;
    /** Returns true if argument is true */
    bool isDiscreteValued(std::vector<bool> const &mask) const;
};


#endif /* FUNC_ABS_H_ */
#ifndef FUNC_NOT_H_
#define FUNC_NOT_H_

#include <function/ScalarFunction.h>

namespace base {

/**
 * @short Unary minus operator
 * UnaryMinus implements the prefix operator "-"
 * y <- -x
 */
class Not : public ScalarFunction
{
public:
    Not ();
    double evaluate(std::vector<double const *> const &args) const;
    bool isDiscreteValued(std::vector<bool> const &mask) const;
    std::string deparse(std::vector<std::string> const &par) const;
};

}

#endif /* FUNC_NOT_H_ */

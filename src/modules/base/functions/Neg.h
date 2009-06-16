#ifndef FUNC_NEG_H_
#define FUNC_NEG_H_

#include <function/ScalarFunc.h>

namespace base {

/**
 * @short Unary minus operator
 * UnaryMinus implements the prefix operator "-"
 * y <- -x
 */
class Neg : public ScalarFunc 
{
public:
    Neg ();
    double evaluateScalar(std::vector<double const *> const &args) const;
    bool isDiscreteValued(std::vector<bool> const &mask) const;
    bool isScale(std::vector<bool> const &mask, 
		 std::vector<bool> const &fix) const;
    std::string deparse(std::vector<std::string> const &par) const;

};

}

#endif /* FUNC_NEG_H_ */

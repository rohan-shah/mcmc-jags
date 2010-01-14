#ifndef FUNC_ADD_H_
#define FUNC_ADD_H_

#include "Infix.h"

namespace base {

/**
 * The Add function implements the infix operator "+" in the BUGS language.
 * The expression a + b + c is evaluated in a single function call.
 * @short Sum of scalars
 * <pre>
 * y <- a + b
 * </pre>
 */
class Add : public Infix
{
public:
    Add ();
    double evaluate(std::vector<double const *>const &args) const;
    bool isDiscreteValued(std::vector<bool> const &flags) const;
    bool isLinear(std::vector<bool> const &mask,
		  std::vector<bool> const &fixmask) const;
    bool isScale(std::vector<bool> const &mask,
		 std::vector<bool> const &fixmask) const;
};

}

#endif /* FUNC_ADD_H_ */

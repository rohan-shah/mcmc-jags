#ifndef FUNC_MULTIPLY_H_
#define FUNC_MULTIPLY_H_

#include "Infix.h"

namespace base {

/**
 * @short Product of two scalars 
 * 
 * The Multiply function implements the infix operator "*" in the
 * BUGS language.  The expression a * b * c is broken down into ((a *
 * b) * c), so two separate calls to Multiply are made.
 */
class Multiply: public Infix
{
public:
    Multiply ();
    double evaluateScalar(std::vector<double const *> const &args) const;
    bool isDiscreteValued(std::vector<bool> const &mask) const;
    bool isLinear(std::vector<bool> const &mask,
		  std::vector<bool> const &fixmask) const;
    bool isScale(unsigned int index, std::vector<bool> const &fixmask) const;
};

}

#endif /* FUNC_MULTIPLY_H_ */

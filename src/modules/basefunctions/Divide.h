#ifndef FUNC_DIVIDE_H_
#define FUNC_DIVIDE_H_

#include "Infix.h"

namespace basefunctions {

/**
 * The Divide function implements the infix operator "/" in the BUGS 
 * language
 * @short Ratio of two scalars
 * <pre>
 * y <- a/b
 * y = a/b for b != 0
 * </pre>
 */
class Divide:public Infix
{
public:
    Divide ();
    double eval(std::vector<double const *> const &args) const;
    bool checkParameterValue (std::vector <double const *> const &args) const;
    bool isLinear(std::vector<bool> const &mask,
		  std::vector<bool> const &fixmask) const;
    bool isScale(unsigned int index, std::vector<bool> const &fixmask) const;
};

}

#endif /* FUNC_DIVIDE_H_ */

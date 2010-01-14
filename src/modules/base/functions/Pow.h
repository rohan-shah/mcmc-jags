#ifndef FUNC_POW_H_
#define FUNC_POW_H_

#include "Infix.h"

namespace base {
    
/**
 * @short Power function using infix notation
 * If y <- x^z and x is negative, then z must be an integer
 */
class Pow : public Infix
{
public:
    Pow ();
    double evaluate(std::vector<double const *> const &args) const;
    bool checkParameterValue(std::vector<double const*> const &args) const;
    bool isPower(std::vector<bool> const &mask, 
		 std::vector<bool> const &fix) const;
};

}

#endif /* FUNC_POW_H_ */

#ifndef FUNC_AND_H_
#define FUNC_AND_H_

#include "Infix.h"

namespace base {

/**
 * The And function implements the infix operator "&" in the BUGS 
 * language
 * @short Logical and 
 * <pre>
 * y <- a&b
 * y = 1 if a != 0 and  b != 0
 * y = 0 otherwise
 * </pre>
 */
class And : public Infix
{
public:
    And ();
    double evaluate(std::vector<double const *> const &args) const;
    bool isDiscreteValued(std::vector<bool> const &mask) const;
};

}

#endif /* FUNC_AND_H_ */

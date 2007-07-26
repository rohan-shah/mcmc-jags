#ifndef FUNC_MIN_H_
#define FUNC_MIN_H_

#include <function/Function.h>

/**
 * @short Minimum element of a vector
 * @see Max
 */
class Min: public Function
{
public:
  Min ();
  void evaluate(double *x, std::vector<double const *> const &args,
		std::vector<unsigned int> const &lengths,
		std::vector<std::vector<unsigned int> > const &dims) const;
  bool checkParameterDim (std::vector<std::vector<unsigned int> > const &dims)
      const;
  /** Returns true if all arguments are true */
  bool isDiscreteValued(std::vector<bool> const &mask) const;  
};

#endif /* FUNC_MIN_H_ */

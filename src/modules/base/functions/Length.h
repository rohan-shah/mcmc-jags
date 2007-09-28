#ifndef FUNC_LENGTH_H_
#define FUNC_LENGTH_H_

#include <function/Function.h>

namespace base {

/**
 * @short Length of an array
 * Length returns the number of elements in an array
 * @see Mean
 * <pre>
 * y <- length(x[])
 * </pre>
 */
class Length : public Function
{
public:
  Length ();
  void evaluate (double *x, std::vector <double const *> const &args,
		 std::vector<unsigned int> const &lengths,
		 std::vector<std::vector<unsigned int> > const &dims) const;
  bool checkParameterDim(std::vector<std::vector<unsigned int> > const &dims) 
      const;
  bool isDiscreteValued(std::vector<bool> const &mask) const;
};

}

#endif /* FUNC_LENGTH_H_ */

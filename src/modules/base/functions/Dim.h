#ifndef FUNC_DIM_H_
#define FUNC_DIM_H_

#include <function/Function.h>

namespace base {

/**
 * @short  of an array
 * The dim function returns the dimensions of an array
 * <pre>
 * d <- dim(x)
 * </pre>
 */
class Dim : public Function
{
public:
  Dim ();
  void evaluate (double *x, std::vector <double const *> const &args,
		 std::vector<unsigned int> const &lengths,
		 std::vector<std::vector<unsigned int> > const &dims) const;
  std::vector<unsigned int> 	
      dim(std::vector <std::vector<unsigned int> > const &dims) const;
  bool checkParameterDim(std::vector<std::vector<unsigned int> > const &dims) 
      const;
  bool isDiscreteValued(std::vector<bool> const &mask) const;
};

}
#endif /* FUNC_DIM_H_ */

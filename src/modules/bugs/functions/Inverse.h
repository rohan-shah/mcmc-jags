#ifndef INVERSE_H_
#define INVERSE_H_

#include <function/Function.h>

/**
 * @short Inverts a square matrix
 * <pre>
 * y[,] <- inverse(x[,])
 * </pre>
 */
class Inverse:public Function
{
public:
  Inverse ();

  void evaluate (SArray & value,
		 std::vector <SArray const *> const &args) const;
  std::vector<unsigned int> dim (std::vector<std::vector<unsigned int> > const &args) const;
  bool checkParameterDim (std::vector<std::vector<unsigned int> > const &dims) const;
};

#endif /* INVERSE_H_ */

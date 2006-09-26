#ifndef MEXP_H_
#define MEXP_H_

#include <function/Function.h>

class Mexp : public Function
{
 public:
  Mexp();
  void evaluate (SArray &value,
		 std::vector <SArray const *> const &args) const;
  std::vector<unsigned int> dim (std::vector<std::vector<unsigned int> > const &dims) const;
  bool checkParameterDim (std::vector <std::vector<unsigned int> > const &dims) const;
};

#endif /* MEXP_H_ */

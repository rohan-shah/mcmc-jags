#ifndef MAT_MULT_H_
#define MAT_MULT_H_

#include <function/Function.h>

class MatMult : public Function
{
public:
    MatMult();
    void evaluate (double *value, std::vector<double const *> const &args,
		   std::vector<unsigned int> const &lengths,
		   std::vector<std::vector<unsigned int> > const &dims) const;
    std::vector<unsigned int> 
	dim(std::vector<std::vector<unsigned int> > const &dims) const;
    bool checkParameterDim(std::vector <std::vector<unsigned int> > const &dims) const;
    bool isLinear(std::vector<bool> const &mask, std::vector<bool> const &fix) const;
	
};

#endif /* MAT_MULT_H_ */

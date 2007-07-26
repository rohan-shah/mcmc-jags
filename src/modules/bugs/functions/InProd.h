#ifndef FUNC_INPROD_H_
#define FUNC_INPROD_H_

#include <function/Function.h>

/**
 * @short Inner product of two vectors
 * The InProd function returns the sum of the product of corresponding
 * elements from two arrays with the same dimension
 * <pre>
 * y <- inprod(a[],b[])
 * </pre>
 */
class InProd : public Function
{
public:
    InProd ();
    void evaluate (double *value,
		   std::vector <double const *> const &args,
		   std::vector<unsigned int> const &lengths,
		   std::vector<std::vector<unsigned int> > const &dims) const;
    bool checkParameterDim (std::vector <std::vector<unsigned int> > const &dims) const;
    bool isDiscreteValued(std::vector<bool> const &mask) const;
    bool isLinear(std::vector<bool> const &mask,
		  std::vector<bool> const &fix) const;
    bool isScale(unsigned int index, std::vector<bool> const &fix) const;
};

#endif /* FUNC_INPROD_H_ */

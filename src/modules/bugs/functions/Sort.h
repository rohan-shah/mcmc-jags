#ifndef FUNC_SORT_H_
#define FUNC_SORT_H_

#include <function/Function.h>

/**
 * @short Sort function
 * Sorts the elements of a vector in ascending order
 * <pre>
 * y <- sort(x[])
 * </pre>
 */
class Sort : public Function
{
public:
    Sort ();
    void evaluate(double *value, std::vector <double const *> const &args,
		  std::vector <unsigned int> const &lengths,
		  std::vector<std::vector<unsigned int> > const &dims) const;
    bool checkParameterDim(std::vector<std::vector<unsigned int> > const &dims)
	const;
    std::vector<unsigned int>  
	dim(std::vector <std::vector<unsigned int> > const &dims) const;
    bool isDiscreteValued(std::vector<bool> const &mask) const;
};

#endif /* FUNC_SORT_H_ */

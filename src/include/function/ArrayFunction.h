#ifndef ARRAY_FUNCTION_H_
#define ARRAY_FUNCTION_H_

#include <function/Function.h>

#include <string>
#include <vector>

/**
 * @short Matrix- or array-valued function
 *
 * Array-valued functions are the most general class of function. The
 * arguments of an array-valued function, and the value, may be
 * scalars, vector, matrices or arrays.
 *
 */
class ArrayFunction : public Function
{
public:
    /**
     * Constructor.
     */
    ArrayFunction(std::string const &name, unsigned int npar);
    /**
     * Evaluates the function. 
     *
     * @param value array of doubles which contains the result of
     * the evaluation on exit 
     * @param args Vector of arguments. 
     * @param dims Respective dimensions of each element of args.
     */
    virtual void
	evaluate(double *value, 
		 std::vector<double const *> const &args,
		 std::vector<std::vector<unsigned int> > const &dims) const = 0;
    /**
     * Checks whether dimensions of the function parameters are correct.  
     *
     * @param dims Vector of length npar denoting the dimensions of
     * the parameters, with any redundant dimensions dropped.
     */
    virtual bool
	checkParameterDim(std::vector<std::vector<unsigned int> > const &dims) 
	const = 0;
    /**
     * Checks whether the parameter values lie in the domain of the
     * function. The default implementation returns true.
     */
    virtual bool
	checkParameterValue(std::vector<double const *> const &args,
                            std::vector<std::vector<unsigned int> > const &dims) 
        const;
    /**
     * Calculates what the dimension of the return value should be,
     * based on the dimensions of the arguments. The dimension of the
     * return value cannot depend on the value of any of the arguments,
     * only their dimensions.
     *
     * @param dims Vector of Indices denoting the dimensions of the
     * parameters. This vector must return true when passed to
     * checkParameterDim.
     */
    virtual std::vector<unsigned int> 
	dim(std::vector <std::vector<unsigned int> > const &dims) const = 0;
};

#endif /* ARRAY_FUNCTION_H_ */

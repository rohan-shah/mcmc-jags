#ifndef VECTOR_FUNCTION_H_
#define VECTOR_FUNCTION_H_

#include <function/Function.h>

/**
 * @short Vector-valued function with vector arguments
 *
 * VectorFunc represents vector-valued functions whose parameters are
 * also vectors.
 */
class VectorFunction : public Function
{
public:
    VectorFunction(std::string const &name, unsigned int npar);
    /**
     * Evaluates the function and writes the result into the given
     * array
     *
     * @param value Array of doubles which will contain the result on exit.
     * @param args Vector of arguments
     * @param lengths Vector of argument lengths: the length of the array
     * of doubles pointed to by args[i] is lengths[i]. 
     */
    virtual void evaluate(double *value, 
			  std::vector <double const *> const &args,
			  std::vector <unsigned int> const &lengths) const = 0;
    /**
     * Calculates the length of the return value based on the lengths of
     * the arguments. The argument must return true when passed to
     * checkParameterLength. The default member function assumes that 
     * the VectorFunction returns a scalar value.
     */
    virtual unsigned int 
	length(std::vector <unsigned int> const &arglengths) const;
    /**
     * Checks that the lengths of all the arguments are consistent. 
     * The default implementation returns true.
     */
    virtual bool 
	checkParameterLength(std::vector<unsigned int> const &args) const;
    /**
     * Checks that the values of the arguments are in the domain of
     * the function. The default implementation returns true.
     */
    virtual bool 
	checkParameterValue(std::vector<double const *> const &args,
			    std::vector<unsigned int> const &lengths) const; 
};

#endif /* VECTOR_FUNCTION_H_ */

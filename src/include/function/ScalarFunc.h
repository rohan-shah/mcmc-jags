#ifndef SCALAR_FUNC_H_
#define SCALAR_FUNC_H_

#include <function/Function.h>

/** 
 * @short Scalar-valued Function with scalar arguments
 *
 * ScalarFunc is a convenience class for scalar-valued functions whose
 * parameters are also scalars. It provides simplified versions of some
 * of the virtual functions defined by the Function class.
 */
class ScalarFunc : public Function
{
public:
    ScalarFunc(std::string const &name, unsigned int npar);
    /** 
     * Implementation of the virtual function Function#evaluate, which
     * calls ScalarFunc#eval.
     */
    void evaluate(double *value, 
		  std::vector <double const *> const &args,
		  std::vector <unsigned int> const &lengths,
                  std::vector<std::vector<unsigned int> > const &dims) const;
    /**
     * Scalar functions need to implement this simplified function,
     * instead of Function#evaluate.
     */
    virtual double eval(std::vector <double const *> const &args) const = 0;
    /**
     * Returns (1) since the result of a scalar function should
     * always be scalar.
     */
    std::vector<unsigned int> 
	dim(std::vector <std::vector<unsigned int> > const &args) const;
    /**
     * Checks that all arguments are scalar.
     */
    bool checkParameterDim(std::vector<std::vector<unsigned int> > const &args)
	const;
    /**
     * Implementation of Function#checkParameterValues which calls the
     * simplified version below.
     */
    bool checkParameterValue(std::vector<double const *> const &args,
                             std::vector<unsigned int> const &lengths,
			     std::vector<std::vector<unsigned int> > const &dim)
	const;
    /**
     * Simplified version of checkParameterValues for Scalar functions.
     *
     * The default version always returns true.
     */
    virtual bool 
	checkParameterValue(std::vector<double const *> const &args) const;
};

#endif /* SCALAR_FUNC_H_ */

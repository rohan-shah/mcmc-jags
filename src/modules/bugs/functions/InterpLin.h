#ifndef FUNC_INTERP_LIN_H_
#define FUNC_INTERP_LIN_H_

#include <function/Function.h>

/**
 * @short Linear interpolation function
 * <pre>
 * y <- interp.lin(e,v1,v2)
 * </pre>
 */
class InterpLin : public Function
{
public:
    InterpLin ();
    void evaluate(double *value, 
		  std::vector<double const *> const &args,
		  std::vector<unsigned int> const &lengths,
		  std::vector<std::vector<unsigned int> > const &dims) const;
    bool checkParameterDim(std::vector<std::vector<unsigned int> > const &dims)
	const;
    bool checkParameterValue(std::vector <double const *> const &args,
			     std::vector<std::vector<unsigned int> > const &dims) 
        const;
};

#endif /* FUNC_INTERP_LIN_H_ */

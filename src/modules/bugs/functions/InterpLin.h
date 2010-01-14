#ifndef FUNC_INTERP_LIN_H_
#define FUNC_INTERP_LIN_H_

#include <function/VectorFunction.h>

namespace bugs {

    /**
     * @short Linear interpolation function
     * <pre>
     * y <- interp.lin(e,v1,v2)
     * </pre>
     */
    class InterpLin : public VectorFunction
    {
    public:
	InterpLin ();
	void evaluate(double *value, std::vector<double const *> const &args,
		      std::vector<unsigned int> const &lengths) const;
	bool checkParameterLength(std::vector<unsigned int> const &len) const;
	bool checkParameterValue(std::vector <double const *> const &args,
				 std::vector <unsigned int> const &lengths)
	    const;
    };

}

#endif /* FUNC_INTERP_LIN_H_ */

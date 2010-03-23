#ifndef P_FUNCTION_H_
#define P_FUNCTION_H_

#include "DPQFunction.h"

namespace bugs {

    class PFunction : public DPQFunction
    {
    public:
	PFunction(RScalarDist const *dist);
	bool checkParameterValue(std::vector<double const *> const &args) const;
	double evaluate(std::vector <double const *> const &args) const;
    };

}

#endif /* P_FUNCTION_H_ */

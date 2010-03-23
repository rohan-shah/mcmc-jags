#ifndef Q_FUNCTION_H_
#define Q_FUNCTION_H_

#include "DPQFunction.h"

namespace bugs {

    class QFunction : public DPQFunction
    {
    public:
	QFunction(RScalarDist const *dist);
	bool checkParameterValue(std::vector<double const *> const &args) const;
	double evaluate(std::vector <double const *> const &args) const;
    };

}

#endif /* Q_FUNCTION_H_ */

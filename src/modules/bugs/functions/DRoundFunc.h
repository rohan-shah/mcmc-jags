#ifndef FUNC_DROUND_H_
#define FUNC_DROUND_H_

#include <function/ScalarFunction.h>

namespace bugs {

    /**
     * @short Function version of dround distribution
     */
    class DRoundFunc: public ScalarFunction
    {
    public:
	DRoundFunc ();
	double evaluate (std::vector <double const *> const &args) const;
    };

}

#endif /* FUNC_DROUND_H_ */

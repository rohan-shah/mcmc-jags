#ifndef FUNC_ICLOGLOG_H_
#define FUNC_ICLOGLOG_H_

#include <function/LinkFunction.h>

namespace bugs {

    /**
     * @short inverse complementary log log link
     * @see CLogLog
     * <pre>
     * cloglog(y) <- a + b*x
     * y <- icloglog(a + b*x)
     * </pre>
     */
    class ICLogLog : public LinkFunction
    {
    public:
	ICLogLog ();
	double inverseLink(double eta) const;
	double link(double mu) const;
	double grad(double eta) const;
    };

}

#endif /* FUNC_ICLOGLOG_H_ */

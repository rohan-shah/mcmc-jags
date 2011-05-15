#ifndef DNCHISQ_H_
#define DNCHISQ_H_

#include "RScalarDist.h"

namespace bugs {

    /** 
     * @short Non-central Chi square distribution
     * <pre>
     * x ~ dnchisqr(k, delta)
     * f(x | k, delta) = exp(-delta/2) 
     *                   SUM_{r=0}^Inf ((delta/2)^r / r!) dchisq(x, k + 2r)
     * </pre>
     */
    class DNChisqr : public RScalarDist {
    public:
	DNChisqr();

	double d(double x, PDFType type,
		 std::vector<double const *> const &parameters, 
		 bool give_log) const;
	double p(double q, std::vector<double const *> const &parameters, 
		 bool lower, bool give_log) const;
	double q(double p, std::vector<double const *> const &parameters, 
		 bool lower, bool log_p) const;
	double r(std::vector<double const *> const &parameters, RNG *rng) const;
	bool checkParameterValue(std::vector<double const *> const &parameters) const;
	std::string alias() const;
    };

}

#endif /* DNCHISQ_H_ */

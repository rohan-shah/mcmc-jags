#ifndef DNT_H_
#define DNT_H_

#include <distribution/RScalarDist.h>

namespace jags {
    namespace bugs {

	/**
	 * Non-central t-distribution on k degrees of freedom, with
	 * location/non-centrality parameter parameter mu, and
	 * precision parameter tau.
	 * 
	 * The non-central t has no simple closed-form expression for
	 * its density but it can be defined constructively in terms
	 * of underlying normal and chi-squared distributions.
	 *
	 * <pre>
         * X ~ dnt(mu, tau, k)
	 * X = U/sqrt(V/k))
	 * U ~ N(mu, tau)
	 * V ~ Chi2(k)
	 * </pre>
	 *
	 * The non-central t-distribution is normally only defined for
	 * tau=1, in which case mu is the non-centrality
	 * parameter. The 3-parameter form here is a scaled version of
	 * a standard non-central t with non-centrality parameter
	 * delta = mu * sqrt(tau).
	 *
	 * @short non-central t distribution
	 */
	class DNT : public RScalarDist {
	  public:
	    DNT();
	    double d(double x, PDFType type,
		     std::vector<double const *> const &parameters, 
		     bool log) const;
	    double p(double x, std::vector<double const *> const &parameters,
		     bool lower, bool log) const;
	    double q(double x, std::vector<double const *> const &parameters,
		     bool lower, bool log) const;
	    double r(std::vector<double const *> const &parameters,
		     RNG *rng) const;
	    /**
	     * Check that tau > 0 and k > 0 also that the non-centrality
	     * parameter delta = mu * sqrt(tau) is no larger than 37.62
	     */
	    bool checkParameterValue(std::vector<double const *> const &
				     parameters) const;
	};
    }
}

#endif /* DNT_H_ */

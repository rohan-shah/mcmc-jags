#ifndef DDEXP_H_
#define DDEXP_H_

#include "RScalarDist.h"

namespace bugs {

/**
 * @short Double exponential distribution
 * <pre>
 * x ~ ddexp(mu,tau)
 * f(x|mu,tau) = (tau/2) * exp(-tau * abs(x-mu))
 * </pre>
 */
class DDexp : public RScalarDist {
public:
  DDexp();
  
  double d(double x, PDFType type,
	   std::vector<double const *> const &parameters, 
	   bool give_log) const;
  double p(double q, std::vector<double const *> const &parameters, bool lower,
	   bool give_log) const;
  double q(double p, std::vector<double const *> const &parameters, bool lower,
	   bool log_p) const;
  double r(std::vector<double const *> const &parameters, RNG *rng) const;
  /**
   * Checks that tau > 0
   */
  bool checkParameterValue(std::vector<double const *> const &parameters) const;
  double KL(std::vector<double const *> const &par1,
	    std::vector<double const *> const &par2) const;
};

}

#endif /* DDEXP_H_ */

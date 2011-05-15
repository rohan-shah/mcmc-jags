#ifndef DGEN_GAMMA_H_
#define DGEN_GAMMA_H_

#include "RScalarDist.h"

namespace bugs {

/**
 * @short Generalizedgamma distribution
 * <pre>
 * X ~ dgen.gamma(r, mu, beta)
 * f(x|r,mu) = beta * mu^(beta*r) * x^(beta*r - 1) * exp(-(mu * x)^beta) / gamma(r)
 * </pre>
 */
class DGenGamma : public RScalarDist {
 public:
  DGenGamma();
  std::string alias() const;
  double d(double x, PDFType type,
	   std::vector<double const *> const &parameters, 
	   bool give_log) const;
  double p(double q, std::vector<double const *> const &parameters, bool lower,
	   bool give_log) const;
  double q(double p, std::vector<double const *> const &parameters, bool lower,
	   bool log_p) const;
  double r(std::vector<double const *> const &parameters, RNG *rng) const;
  /**
   * Checks that r > 0, mu > 0, beta > 0
   */
  bool checkParameterValue(std::vector<double const *> const &parameters) const;
  double KL(std::vector<double const *> const &par1,
	    std::vector<double const *> const &par2) const;
};

}

#endif /* DGEN_GAMMA_H_ */

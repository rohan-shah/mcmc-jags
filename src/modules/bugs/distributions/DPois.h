#ifndef DPOIS_H_
#define DPOIS_H_

#include "RScalarDist.h"

namespace bugs {

/**
 * <pre>
 * r ~ dpois(lambda)
 * f(r|lambda) = exp(-lambda) * lambda^r / r! ; r = 0, 1 ,...
 * </pre>
 * @short Poisson distribution
 */
class DPois : public RScalarDist {
 public:
  DPois();

  double d(double x, PDFType type,
	   std::vector<double const *> const &parameters, 
	   bool give_log) const;
  double p(double q, std::vector<double const *> const &parameters, bool lower,
	   bool give_log) const;
  double q(double p, std::vector<double const *> const &parameters, bool lower,
	   bool log_p) const;
  double r(std::vector<double const *> const &parameters, RNG *rng) const;
  /**
   * Checks that lambda > 0
   */
  bool checkParameterValue(std::vector<double const *> const &parameters) const;
  double KL(std::vector<double const *> const &par1,
	    std::vector<double const *> const &par2) const;
};

}

#endif /* DPOIS_H_ */

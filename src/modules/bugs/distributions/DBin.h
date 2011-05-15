#ifndef DBIN_H_
#define DBIN_H_

#include "RScalarDist.h"

namespace bugs {

/**
 * @short Binomial distribution
 * <pre>
 * R ~ dbin(p, n)
 * f(r|p,n) = n!/r!.(n - r)! p^r * (1 - p)^(n - r) ; r in 0:n
 * </pre>
 */
class DBin : public RScalarDist {
 public:
  DBin();
  std::string alias() const;
  double d(double x, PDFType type,
	   std::vector<double const *> const &parameters, 
	   bool give_log) const;
  double p(double x, std::vector<double const *> const &parameters, bool lower,
	   bool give_log) const;
  double q(double p, std::vector<double const *> const &parameters, bool lower,
	   bool log_p) const;
  double r(std::vector<double const *> const &parameters, RNG *rng) const;
  double l(std::vector<double const *> const &parameters) const;
  double u(std::vector<double const *> const &parameters) const;
  /**
   * Checks that n is discrete-valued.
   */
  bool checkParameterDiscrete (std::vector<bool> const &mask) const;
  /**
   * Checks that p lies in (0,1) and n > 1
   */
  bool checkParameterValue(std::vector<double const *> const &parameters) const;
  bool isSupportFixed(std::vector<bool> const &fixmask) const;
  double KL(std::vector<double const *> const &par1, 
	    std::vector<double const *> const &par2) const;
};

}

#endif /* DBIN_H_ */

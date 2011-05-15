#ifndef DHYPER_H_
#define DHYPER_H_

#include "RScalarDist.h"

namespace bugs {

/**
 * @short Hypergeometric distribution
 * <pre>
 * R ~ dhyper(n1,n2,m1,psi)
 * </pre>
 */
class DHyper : public RScalarDist {
 public:
  DHyper();

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
   * Checks that n1, n2, m1 are discrete
   */
  bool checkParameterDiscrete (std::vector<bool> const &mask) const;
  bool checkParameterValue(std::vector<double const*> const &parameters) const;
  bool isSupportFixed(std::vector<bool> const &fixmask) const;
  bool canBound() const;
  double KL(std::vector<double const *> const &par1,
	    std::vector<double const *> const &par2) const;
};

}

#endif /* DHYPER_H_ */

#ifndef DNEGBIN_H_
#define DNEGBIN_H_

#include "RScalarDist.h"

namespace bugs {

/**
 * <pre>
 * x ~ dnegbin(p, r)
 * f(x|p,r) = ((x+r-1)!/(x!*(r-1)!)) * p^r * (1-p)^x
 * </pre>
 * @short Negative Binomial distribution
 */
class DNegBin : public RScalarDist {
 public:
  DNegBin();
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
   * Checks that p lies in the interval (0,1) and r > 0
   */
  bool checkParameterValue(std::vector<double const *> const &parameters) const;
  double KL(std::vector<double const *> const &par1,
	    std::vector<double const *> const &par2) const;
};

}

#endif /* DNEGBIN_H_ */

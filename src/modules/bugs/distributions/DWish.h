#ifndef DWISH_H_
#define DWISH_H_

#include <distribution/Distribution.h>

/**
 * <pre>
 * x[] ~ dwish(R[,], k)
 * </pre>
 * @short Wishart distribution
 */
class DWish : public Distribution {
public:
  DWish();
  ~DWish();

  double logLikelihood(SArray const &x,
		       std::vector<SArray const *> const &parameters) const;
  void randomSample(SArray &x, std::vector<SArray const *> const &parameters,
		    RNG *rng) const;
  static void randomSample(double *x, unsigned int length,
                           double const *R, double k, int nrow,
                           RNG *rng);

  /**
   * Checks that R is a square matrix and k is a scalar
   */
  bool checkParameterDim (std::vector<std::vector<unsigned int> > const &dims) const;
  /**
   * Checks that R is symmetric and k >= nrow(R). There is
   * currently no check that R is positive definite
   */
  bool checkParameterValue (std::vector<SArray const *> const &parameters) const;
  std::vector<unsigned int> dim(std::vector<std::vector<unsigned int> > const &dims) const;
  unsigned int df(std::vector<SArray const *> const &parameters) const;
  double lowerSupport(unsigned int i,
		      std::vector<SArray const *> const &parameters) const;
  double upperSupport(unsigned int i,
		      std::vector<SArray const *> const &parameters) const;
  void typicalValue(SArray &x, std::vector<SArray const *> const &parameters)
    const;
};

#endif /* DWISH_H_ */

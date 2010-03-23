#ifndef ARRAY_DIST_H_
#define ARRAY_DIST_H_

#include <distribution/Distribution.h>

#include <vector>
#include <string>

struct RNG;

/**
 * @short Distribution of a random variable
 *
 * Distribution objects contain only constant data members and all
 * member functions are constant. Hence only one object needs to be
 * instantiated for each subclass.
 */
class ArrayDist : public Distribution
{
public:
    /**
     * Constructor.
     * @param name name of the distribution as used in the BUGS language
     * @param npar number of parameters, excluding upper and lower bounds
     */
    ArrayDist(std::string const &name, unsigned int npar);
    /**
     * @param x Value at which to evaluate the likelihood.
     *
     * @param dim Dimensions of the value
     *
     * @param parameters Vector of parameter values of the
     * distribution.
     * 
     * @param dims Dimensions of the parameters.
     *
     * @returns The log likelihood.  If the likelihood should be zero
     * because x is inconsistent with the parameters then -Inf is
     * returned. If the parameters are invalid
     * (i.e. checkParameterValue returns false), then the return value
     * is undefined.
     */
    virtual double 
	logLikelihood(double const *x, unsigned int length,
		      std::vector<double const *> const &parameters,
		      std::vector<std::vector<unsigned int> > const &dims,
		      double const *lbound, double const *ubound)
	const = 0;
    /**
     * Draws a random sample from the distribution. 
     *
     * @param x Array to which the sample values are written
     *
     * @param parameters Vector of parameter values at which to
     * evaluate the likelihood. This vector should be of length npar(). 
     *
     * @param rng pseudo-random number generator to use.
     *
     * @exception length_error 
     */
    virtual void 
	randomSample(double *x, unsigned int length,
		     std::vector<double const *> const &parameters,
		     std::vector<std::vector<unsigned int> > const  &dims,
		     double const *lbound, double const *ubound,
		     RNG *rng) 	const = 0;
    /**
     * Returns a typical value from the distribution.  The meaning of
     * this will depend on the distribution, but it will normally be a
     * mean, median or mode.
     *
     * @param x Array to which the sample values are written
     *
     * @param parameters  Vector of parameter values at which
     * to evaluate the likelihood. This vector should be of length
     * npar().
     *
     * @param dims Vector of parameter dimensions.
     *
     * @exception length_error 
     */
    virtual void 
	typicalValue(double *x, unsigned int length,
		     std::vector<double const *> const &parameters,
		     std::vector<std::vector<unsigned int> > const &dims,
		     double const *lbound, double const *ubound)
	const = 0;
    /**
     * Checks that dimensions of the parameters are correct.
     *
     * This function only needs to be run once for each parameter
     * vector. Thereafter, the values of the parameters will change,
     * but the dimensions will not.
     */
    virtual bool 
	checkParameterDim (std::vector<std::vector<unsigned int> > const &parameters) 
	const = 0;
    /**
     * Checks that the values of the parameters are consistent with
     * the distribution. For example, some distributions require
     * than certain parameters are positive, or lie in a given
     * range.
     *
     * This function assumes that checkParameterDim returns true.
     */
    virtual bool 
	checkParameterValue(std::vector<double const *> const &parameters,
			    std::vector<std::vector<unsigned int> > const &dims) const = 0;
    /**
     * Calculates what the dimension of the distribution should be,
     * based on the dimensions of its parameters. 
     */
    virtual std::vector<unsigned int> 
	dim (std::vector <std::vector<unsigned int> > const &args) const = 0;
    /**
     * Returns the number of degrees of freedom of the distribution
     * given the dimensions of the parameters. By default this is the
     * product of the elements of the dimension vector returned by
     * ArrayDist#dim. However, some distributions are constrained: and
     * the support occupies a lower dimensional subspace. In this
     * case, the df member function must be overrideen.
     */
    virtual unsigned int df(std::vector<std::vector<unsigned int> > const &dims)
	const;
    /**
     * Returns the support of an unbounded distribution
     */
    virtual void 
	support(double *lower, double *upper, unsigned int length,
		std::vector<double const *> const &support,
		std::vector<std::vector<unsigned int> > const &dims) const = 0;
};

#endif /* DISTRIBUTION_H_ */

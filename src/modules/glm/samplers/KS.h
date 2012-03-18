#ifndef KS_H_
#define KS_H_

class RNG;

namespace glm {
    /**
     * Utility function used by the HolmesHeld and AlbertChib sampling
     * methods in binary logistic regression models.
     *
     * A variable "Z" with logistic distribution may be considered as
     * a scale mixture of normal distributions with variance lambda,
     * where lambda=(2*psi)^2 and "psi" has a Kolmogorov-Smirnov
     * distribution. Given Z, this function draws a sample from the
     * posterior distribution of lambda.
     *
     * Although the posterior distribution of lambda given Z cannot
     * be expressed in closed form, it can be efficiently sampled using
     * a series approximation as described by Devroye (1986) Non-Uniform
     * Random Variate Generation, Springer-Verlag, New York. Note that
     * this book is currently available for free on line at
     * http://luc.devroye.org/rnbookindex.html
     *
     * @param Z value of a random variable with logistic distribution
     *
     * @param rng Random number generator used for sampling
     */
    double sample_lambda(double Z, RNG *rng);
}

#endif /* KS_H_ */

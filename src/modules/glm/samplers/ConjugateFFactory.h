#ifndef CONJUGATE_F_FACTORY_H_
#define CONJUGATE_F_FACTORY_H_

#include <sampler/SingletonFactory.h>

namespace glm {

  /**
   * @short Factory for Conjugate F Sampler 
   *
   * @see ConjugateFSampler
   */
  class ConjugateFFactory : public SingletonFactory
  {
  public:
    ~ConjugateFFactory();
    /**
     * If a stochastic node is the precision hyper-parameter of one or
     * more parameters in a linear model, with an F(m, 1) distribution
     * for fixed m, then it may be sampled by a ConjugateFSampler.
     * Scale transformations are also permitted.
     *
     * ConjugateFSampler works by marginalizing over the coefficients
     * of the linear model. It requires that these coefficients have
     * unbounded normal distributions.
     */
    bool canSample(StochasticNode *snode, Graph const &graph) const;
    /**
     * Creates a ConjugateFSampler for a stochastic node. 
     */
    Sampler *makeSampler(StochasticNode *snode, Graph const &graph) const;
    /**
     * returns "glm::ConjugateF"
     */
    std::string name() const;
  };

}

#endif /* CONJUGATE_F_FACTORY_H_ */

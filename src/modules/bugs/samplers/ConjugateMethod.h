#ifndef CONJUGATE_METHOD_H_
#define CONJUGATE_METHOD_H_

#include <string>

class ConjugateSampler;
class RNG;
class Graph;

class ConjugateMethod
{
public:
    virtual ~ConjugateMethod();
    virtual void initialize(ConjugateSampler *sampler, Graph const &graph) = 0;
    virtual void update(ConjugateSampler *sampler, unsigned int chain, 
			RNG *rng) const = 0;
    virtual std::string name() const = 0;
};

#endif /* CONJUGATE_METHOD_H_ */

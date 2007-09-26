#ifndef CONJUGATE_METHOD_H_
#define CONJUGATE_METHOD_H_

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
};

#endif /* CONJUGATE_METHOD_H_ */

#ifndef CONJUGATE_METHOD_H_
#define CONJUGATE_METHOD_H_

class ConjugateSampler;
class RNG;

class ConjugateMethod
{
public:
    virtual ~ConjugateMethod();
    virtual void initialize(ConjugateSampler *sampler) = 0;
    virtual void update(ConjugateSampler *sampler, unsigned int chain, 
			RNG *rng) const = 0;
};

#endif /* CONJUGATE_METHOD_H_ */

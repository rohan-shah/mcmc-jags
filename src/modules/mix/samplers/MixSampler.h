#include <sampler/Sampler.h>

#include <vector>

class MixSampler : public Sampler
{
    const unsigned int _length;
    double const *_step;
    double const *_value;
public:
    MixSampler();
    void update(RNG *rng);
    void burninOff();
    void setValue(double const *value, unsigned int length);
};

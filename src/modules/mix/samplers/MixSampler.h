#include <sampler/Sampler.h>

class MixSampler : public Sampler
{
    MixSampler();
    void update(RNG *rng);
    void burninOff();
}

#include <config.h>
#include <sampler/DensityMethod.h>

DensityMethod::DensityMethod()
    : _sampler(0), _chain(0)
{
}

DensityMethod::~DensityMethod()
{
}

void DensityMethod::setData(ParallelDensitySampler *sampler, unsigned int chain)
{
    _sampler = sampler;
    _chain = chain;
}

void DensityMethod::initialize(ParallelDensitySampler *sampler, unsigned int chain)
{
}

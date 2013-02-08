#include <config.h>
#include "nmath.h"
#include <rng/RNG.h>


extern "C" {

    double unif_rand(JRNG *rng)
    {
	return rng->uniform();
    }
    
    double exp_rand(JRNG *rng)
    {
	return rng->exponential();
    }
    
    double norm_rand(JRNG *rng)
    {
	return rng->normal();
    }
  
}


#include <config.h>
#include "DSample.h"
#include <rng/RNG.h>
#include <util/dim.h>
#include <util/nainf.h>

#include <cmath>
#include <algorithm>

using std::vector;
using std::string;
using std::max_element;
using std::log;
using std::min;

static inline double const * PROB(vector<double const *> const &par) {
    return par[0];
}
static inline double  SIZE(vector<double const *> const &par) {
    return *par[1];
}

namespace jags {
    namespace bugs {

	DSample::DSample()
	    : VectorDist("dsample", 2) 
	{}
	
	bool DSample::isDiscreteValued(vector<bool> const &mask) const
	{
	    return true; //Takes only 0, 1 values
	}

	bool DSample::checkParameterLength(vector<unsigned int> const &len) const
	{
	    //Check that PROB is non-empty and SIZE is a scalar
	    return len[0] >= 1 && len[1] == 1;
	}

	bool DSample::checkParameterDiscrete(vector<bool> const &mask) const
	{
	    return mask[1]; //SIZE is discrete-valued
	}

	bool DSample::checkParameterValue(vector<double const *> const &par,
					vector<unsigned int> const &len) const
	{
	    if (SIZE(par) < 0 || SIZE(par) > len[0]) {
		//Too few or too many samples requested
		return false;
	    }
	    
	    for (unsigned int i = 0; i < len[0]; ++i) {
		if (PROB(par)[i] <= 0) {
		    //Zero probablity of being sampled
		    return false;
		}
	    }
	    return true;
	}

	unsigned int DSample::length(vector<unsigned int> const &len) const
	{
	    //Length of sample is the same as length of probability weights
	    return len[0];
	}

	double
	DSample::logDensity(double const *x, unsigned int length, PDFType type,
			  vector<double const *> const &par,
			  vector<unsigned int> const &parlen,
			  double const *lower, double const *upper) const
	{
	    double const *prob = PROB(par); // Vector of probability weights
	    unsigned int T = length; // Length of prob vector
	    
	    double pmax = *max_element(prob, prob + T);
	    double lpmax = log(pmax);

	    double loglik = 0;

	    /* 
	     * Numerator: contribution to log likelihood of all
	     * sampled values
	     */
	    unsigned int K = 0; //Count number of sampled values
	    for (unsigned int t = 0; t < T; ++t) {
		if (x[t] == 1) {
		    loglik += log(prob[t]) - lpmax;
		    ++K;
		}
		else if (x[t] != 0) {
		    return JAGS_NEGINF;
		}
	    }
	    if (SIZE(par) != K) {
		//Number of sampled values inconsistent with parameters
		return JAGS_NEGINF;
	    }

	    if (type == PDF_PRIOR) {
		//No need to calculate denominator
		return loglik;
	    }
	    
	    /* 
	     * Denominator: Recursively calculate contributions over
	     * all possible case sets of size K.
	     */
	    vector<double> f(K + 1, 0);
	    f[0] = 1;
	    for (unsigned t = 0; t < T; ++t) {
		double Ct = prob[t]/pmax;
		for (unsigned int k = min<unsigned int>(K, t+1); k > 0; --k) {
		    f[k] += Ct * f[k-1];
		}
	    }
	    loglik -= log(f[K]);

	    return loglik;
	}


	void DSample::randomSample(double *x, unsigned int length,
				 vector<double const *> const &par,
				 vector<unsigned int> const &len,
				 double const *lower, double const *upper,
				 RNG *rng) const
	{
	    //FIXME: Do something here
	}

	void DSample::support(double *lower, double *upper, unsigned int length,
			    vector<double const *> const &par,
			    vector<unsigned int> const &len) const
	{
	    for (unsigned int i = 0; i < length; ++i) {
		lower[i] = 0;
		upper[i] = 1;
	    }
	}

	void DSample::typicalValue(double *x, unsigned int length,
				 vector<double const *> const &par,
				 vector<unsigned int> const &len,
				 double const *lower, double const *upper) const
	{
	    //FIXME: Do something. Take most popular values
	}

	bool DSample::isSupportFixed(vector<bool> const &fixmask) const
	{
	    return true;
	}
	
	unsigned int DSample::df(vector<unsigned int> const &len) const
	{
	    return len[0];
	} 

    }
}

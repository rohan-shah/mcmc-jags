#include <config.h>
#include "Rep.h"

#include <algorithm>
#include <numeric>

using std::vector;
using std::string;
using std::fill;
using std::copy;
using std::accumulate;

namespace jags {
    namespace bugs {

	Rep::Rep() : VectorFunction("rep", 2)
	{
	}
	
	void Rep::evaluate(double *value, 
			   vector <double const *> const &args,
			   vector <unsigned int> const &lengths) const
	{
	    double const *x = args[0]; //Vector to be replicated
	    double const *times = args[1]; //Number of times to replicate

	    unsigned int len_x = lengths[0]; //Length of x vector
	    unsigned int len_times  = lengths[1]; //Length of times vector
	    
	    if (len_times == 1) {
		//Replicate whole vector
		unsigned int ntimes = static_cast<unsigned int>(times[0]);
		for (unsigned int j = 0; j < ntimes; ++j) {
		    value = copy(x, x + len_x, value);
		}
	    }
	    else {
		//Replicate vector element-wise
		for (unsigned int i = 0; i < len_x; ++i) {
		    unsigned int ntimes = static_cast<unsigned int>(times[i]);
		    fill(value, value + ntimes, x[i]);
		    value += ntimes;
		}
	    }
	}

	unsigned int Rep::length(vector <unsigned int> const &lengths,
				 vector <double const *> const &args) const
	{
	    double const *times = args[1];
	    unsigned int len_times  = lengths[1];
	    unsigned int len_x = lengths[0];
	    
	    double y = 0;
	    if (len_x == 0) {
		y = 0;
	    }
	    else if (len_times == 1) {
		y = len_x * times[0];
	    }
	    else if (len_x > 0){
		y = accumulate(times, times + len_times, 0.0);
	    }
	    
	    return static_cast<unsigned int>(y);
	}

	bool Rep::checkParameterLength(vector<unsigned int> const &len) const
	{
	    return (len[0] == 0) || (len[1] == 1) || (len[1] == len[0]);
	}

        bool Rep::isDiscreteValued(vector<bool> const &mask) const
	{
	    //Output is discrete-valued if the first argument is discrete
	    return mask[0];
	}

        bool Rep::checkParameterDiscrete(vector<bool> const &mask) const
	{
	    //Times vector must be discrete
	    return mask[1];
	}
	
	bool Rep::checkParameterFixed(vector<bool> const &mask) const
	{
	    //Times vector must be fixed
	    return mask[1];
	}

	bool Rep::checkParameterValue(vector<double const *> const &args,
				      vector<unsigned int> const &lens) const
	{
	    //Times vector must be non-negative
	    for (unsigned int i = 0; i < lens[1]; ++i) {
		if (args[1][i] < 0) return false;
	    }
	    return true;
	}

    } /* namespace bugs */
} /* namespace jags */

#include "RngStreamFactory.h"
#include "RngStream.h"

#include <ctime>

using std::vector;
using std::time;

static unsigned int timeseed()
{
    //create default seed based on the current time
    return static_cast<unsigned int>(time(NULL));
}

namespace lecuyer {

    RngStreamFactory::RngStreamFactory()
	: _g(timeseed())
    {
    }

    RngStreamFactory::~RngStreamFactory()
    {
	for (unsigned int i = 0; i < _rngvec.size(); ++i) {
	    delete _rngvec[i];
	}
    }

    vector<RNG *> RngStreamFactory::makeRNGs(unsigned int &n)
    {
	vector<RNG *> ans;
	for (unsigned int i = 0; i < n; ++i) {

	    RNG *rng = new RngStream(_g);
	    _g.resetNextSubstream();
	    
	    _rngvec.push_back(rng);
	    ans.push_back(rng);
	}
	_g.resetNextStream();
	n = 0; //FIXME: This is kind of dumb
	return ans;
    }

    RNG * RngStreamFactory::makeRNG(std::string const &name)
    {
	if (name == "lecuyer::RngStream") {

	     RNG *rng = new RngStream(_g);
	     _g.resetNextStream();
	     _rngvec.push_back(rng);
	     return rng;
	}
	else {
	    return 0;
	}
	
    }
    
}

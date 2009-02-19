#ifndef _RNG_STREAM_H_
#define _RNG_STREAM_H_

#include <rng/RmathRNG.h>

namespace lecuyer {
    
    class RngStream : public RmathRNG {
	bool Anti;
	int IncPrec;
	double Cg[6], Bg[6], Ig[6];
	double U01();
	double U01d();
    public:
	RngStream(unsigned int seed);
    
	void resetStartStream ();
	void resetNextStream ();
	void resetStartSubstream ();
	void resetNextSubstream ();
	void setAntithetic (bool a);
	void increasedPrecis (bool incp);
	void advanceState (long e, long c);

	void init(unsigned int seed);
	bool setState(std::vector<int> const &state);
	void getState(std::vector<int> &state) const;
	double uniform();
    
    };
 
}

#endif /* _RNGSTREAM_H_ */
 


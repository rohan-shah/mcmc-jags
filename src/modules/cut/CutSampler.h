#ifndef CUT_SAMPLER_H_
#define CUT_SAMPLER_H_

#include <sampler/Sampler.h>

class CutMethod;

namespace cut {

    class CutSampler : public Sampler
    {
	std::vector<StochasticNode*> _cutnodes;
	unsigned int _ncut;
	std::vector<CutMethod*> _methods;
    public:
	CutSampler(std::vector<StochasticNode*> const &cutnodes,
		   Graph const &graph);
	~CutSampler();
	unsigned int ncut();
	void update(std::vector<RNG*> const &);
	bool adaptOff();
	bool isAdaptive() const;
	std::string name() const;
	static bool cansample(vector<StochasticNode*> const &cutnodes, 
			      Graph const &graph);
    };
    
}

#endif /* CUT_SAMPLER_H_ */

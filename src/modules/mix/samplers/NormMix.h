#ifndef NORM_MIX_METHOD_H_
#define NORM_MIX_METHOD_H_

#include <sampler/TemperedMetropolis.h>
#include <graph/StochasticNode.h>

#include <vector>

class Graph;
class GraphView;

namespace mix {

/**
 * @short Sample method for normal mixture models
 *
 * NormMix implements a Metropolis-Hastings method for normal
 * mixture models with tempered transitions as recommended for mixture
 * models by Celeux et al (Celeux G, Hurn, M, Robert CP, Computational
 * and inferential difficulties with mixture posterior distributions,
 * J Am Statist Ass, 95, 957-970, 2000).
 */
    class NormMix : public TemperedMetropolis
    {
	GraphView const *_gv;
	unsigned int _chain;
	double *_lower;
	double *_upper;
    public:
	/**
	 * Constructor for Mix Method with tempered transitions
	 *
	 * @param max_level Maximum number of levels 
	 *
	 * @param delta Increment in the log temperature at each
	 * level. The maximum temperature reached is thus exp(nlevel *
	 * delta).
	 *
	 * @param nrep Number of Metropolis-Hastings updates to do at each level
	 */
	NormMix(GraphView const *gv, unsigned int chain);
	~NormMix();
	void getValue(std::vector<double> &value) const;
	void setValue(std::vector<double> const &value);
	double logDensity() const;
	double logJacobian(std::vector<double> const &value) const;
	void step(std::vector<double> &value, double step, RNG *rng) const;
	std::string name() const;
	static bool canSample(std::vector<StochasticNode *> const &snodes);
    };

}

#endif /* NORM_MIX_METHOD_H_ */

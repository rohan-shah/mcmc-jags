#ifndef CONJUGATE_H_
#define CONJUGATE_H_

#include <sampler/Sampler.h>

class StochasticNode;
class LogicalNode;

enum ConjugateDist {
  BERN, BETA, BIN, CAT, CHISQ, DEXP, DIRCH, EXP, GAMMA,
  LNORM, LOGIS, MNORM, MULTI, NEGBIN, NORM, PAR, POIS,
  T, UNIF, WEIB, WISH, OTHERDIST
};

/**
 * Convenience function that converts the name of a distribution into
 * a member of the ConjugateDist enumeration. OTHERDIST is returned
 * if the name cannot be found in the internal table.
 */
ConjugateDist getDist(StochasticNode const *snode);

class ConjugateSampler : public Sampler
{
 protected:
  const unsigned int _chain;
  const ConjugateDist _target_dist;
  std::vector<ConjugateDist> _child_dist;
 public:
  ConjugateSampler(StochasticNode *snode, Graph const &graph, unsigned int chain);
  inline StochasticNode *node() { return nodes().front(); }
  /**
   * Conjugate samplers do not have a burnin mode. This function does nothing.
   */
  void burninOff();
};

#endif /* CONJUGATE_H_ */

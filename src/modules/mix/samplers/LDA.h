#ifndef LDA_H_
#define LDA_H_

#include <sampler/SampleMethodNoAdapt.h>

#include <vector>

namespace jags {
    
    class Graph;
    class GraphView;
    class StochasticNode;
    
    namespace mix {
	/**
	 * @short Collapsed sampler for Latent Dirichlet Allocation
	 * models.
	 */
	class LDA : public SampleMethodNoAdapt {
	    const unsigned int _nTopic, _nWord, _nDoc;
	    double const *_topicHyper;
	    double const *_wordHyper;
	    GraphView const *_gv;
	    const unsigned int _chain;	    
	    std::vector<std::vector<int> > _topicTokens, _wordTokens;
	    std::vector<std::vector<int> > _topicsByWord, _topicsByDoc;
	    std::vector<unsigned int> _docSums, _topicSums;
	    bool _wordsObserved;
	    void rebuildTable();
	  public:
	    /**
	     * Constructor.
	     * 
	     * @param topics Topic indicators with a categorical
	     * distribution, organized hierarchically within
	     * documents. Hence topics.size() is nDoc, the number of
	     * documents, and topics[d] contains the topic indicators
	     * for document d.  These are the nodes that are sampled
	     * by the LDA sampler.
	     * 
	     * @param words Word indicators with a categorical
	     * distribution, organized in the same way as
	     * topics. Hence words[d][i] is the stochastic child of
	     * topics[d][i]. All words must be observed.
	     *
	     * @param topic_priors Vector of length nDoc containing
	     * Dirichlet priors for the topic indicators, one for each
	     * document. All topic priors share a common hyper-prior.
	     *
	     * @param word_priors Vector of length nTopic, the number
	     * of topics, containing Dirichlet priors for the
	     * words. The choice of which element of word_prior to use
	     * as prior for words[d][i] is determined by the value of
	     * topics[d][i]. All word priors share a common
	     * hyper-prior.
	     *
	     * @param gv Pointer to the GraphView within which sampling
	     * takes place. 
	     */
	    LDA(std::vector<std::vector<StochasticNode*> > const &topics,
		std::vector<std::vector<StochasticNode*> > const &words,
		std::vector<StochasticNode*> const &topic_priors,
		std::vector<StochasticNode*> const &word_priors,
		GraphView const *gv, unsigned int chain);
	    void update(RNG *rng);
	    /**
	     * Tests whether a set of topics can be sampled by the LDA
	     * method. The arguments are the same as for the LDA
	     * constructor, except that a Graph is passed as the last
	     * argument instead of a GraphView.
	     */
	    static bool canSample(
		std::vector<std::vector<StochasticNode*> > const &topics,
		std::vector<std::vector<StochasticNode*> > const &words,
		std::vector<StochasticNode*> const &topic_priors,
		std::vector<StochasticNode*> const &word_priors,
		Graph const &graph);
	};
    } 
}

#endif /* LDA_H_ */

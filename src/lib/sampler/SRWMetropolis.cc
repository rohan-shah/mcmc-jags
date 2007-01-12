SRWMetropolis::SRWMetropolis(vector<StochasticNode *> const &nodes, 
			     Graph const &graph, unsigned int chain,
			     double const *value, unsigned int length,
			     double prob, double scale)
    : Metropolis(nodes, graph,chain, value, length, 0.234, scale = 1),
{
    _loglik = logFullConditional();
}

SRWMetropolis::update(RNG *rng)
{
    unsigned int length = length();
    double const * current_value = value();
    double * new_value = new double[length];
    double scale = scale();
    for (unsigned int i = 0; i < length; ++i) {
	new_value[i] = current_value[i] + scale * rng->normal();
    }
    propose(new_value, length);
    double new_loglik = logFullConditional(_chain);
    if (accept(rng, max(1.0, exp(loglik - new_loglik)))) {
	_loglik = new_loglik;
    }
    delete [] new_value;
}

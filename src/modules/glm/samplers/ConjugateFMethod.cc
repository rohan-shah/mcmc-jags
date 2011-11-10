#include <config.h>

#include "ConjugateFMethod.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <graph/DeterministicNode.h>
#include <sampler/Linear.h>
#include <module/ModuleError.h>

#include <set>
#include <vector>
#include <cmath>

#include <JRmath.h>

using std::vector;
using std::set;
using std::sqrt;
using std::string;
using std::fabs;

/* 
   We need to prevent the scale parameter from becoming degenerate.
   These two constants are used by the update member function to avoid
   this

   MIN_SCALE Minimum value of abs(_scale), used to bound it away from zero.
   MAX_RNORM Maximum number of attempts to get random value of _scale in
             range.
*/

#define MIN_SCALE 1.0E-3
#define MAX_RNORM 20

static double getScale(StochasticNode const *snode, unsigned int chain)
{
    //Get precision parameter of normal distribution
    return *snode->parents()[1]->value(chain);
}

static double getMean(StochasticNode const *snode, unsigned int chain)
{
    //Get the mean of a normal distribution
    return *snode->parents()[0]->value(chain);
}

static double getValue(StochasticNode const *snode, unsigned int chain)
{
    //Get the current value of a scalar node
    return *snode->value(chain);
}

static void calCoef(double *coef, GraphView const *gv, unsigned int chain)
{   
    //Calculate the coefficients when the stochastic children in the
    //GraphView are all normal random variables such that the precision
    //is a scale function of the sample node

    const double xold = *gv->nodes()[0]->value(chain);
    vector<StochasticNode const*> const &schildren = gv->stochasticChildren();
    unsigned long nchildren = schildren.size();
    
    for (unsigned int i = 0; i < nchildren; ++i) {
        coef[i] = -getScale(schildren[i], chain);
    }
    double val = xold + 1;
    gv->setValue(&val, 1, chain);
    for (unsigned int i = 0; i < nchildren; ++i) {
        coef[i] += getScale(schildren[i], chain);
    }
    gv->setValue(&xold, 1, chain);
}

namespace glm {

    ConjugateFMethod::ConjugateFMethod(GraphView *gv1, GraphView *gv2, 
				       unsigned int chain)
	: _gv1(gv1), _gv2(gv2), _chain(chain),
	  _scale(1), _tau(gv1->nodes()[0]->value(chain)[0]),
	  _coef(0)
    {
	if(!_gv1->deterministicChildren().empty() && checkScale(_gv1, true)) 
	{
	    //One-off calculation of fixed scale transformation
	    _coef = new double[_gv1->stochasticChildren().size()];
	    calCoef(_coef, _gv1, chain);
	}
    }
    
    ConjugateFMethod::~ConjugateFMethod()
    {
	delete [] _coef;
    }
    
    
    //#include <iostream> //debuggin
    void ConjugateFMethod::update(RNG *rng)
    {
	/* There are three levels 

	   X - variance parameter with F(m,1) prior

           X is represented internally using a redundant parameterization
           X = tau/scale^2
           where tau has a Chi-square distribution on m degrees of freedom
           and scale has a standard normal distribution
           
	   Y - Stochastic children of X, which are normally distributed 
           random effects with precision that is a scale function of X

	   Z - Stochastic Children of Y, which are normally distributed 
           random variables forming a linear model in X
	*/

	/* Reparametrize from Y to scaled Y*/

	vector<StochasticNode const*> const &Ynodes = _gv1->stochasticChildren();
	unsigned int J = Ynodes.size();
	vector<double> scaledY(J);
	for (unsigned int j = 0; j < J; ++j) {
	    scaledY[j] -= getValue(Ynodes[j], _chain) - getMean(Ynodes[j], _chain);
	    scaledY[j] /= _scale; 
	}

	/* 
	   Update _tau, which has a conjugate gamma distribution, conditional
	   on scaledY. (Note that _tau is independent of _scale). 
	*/

	//Prior
	vector<Node const *> const &Xparam = _gv1->nodes()[0]->parents();
	double tau_shape = *Xparam[0]->value(_chain)/2; 
	double tau_rate = 0.5;
    
	//Likelihood 
	double *coef = 0;
	bool empty = _gv1->deterministicChildren().empty();
	if (!empty && _coef == 0) {
	    coef = new double[J];
	    calCoef(coef, _gv1, _chain);
	}
	else {
	    coef = _coef;
	}

	for (unsigned int j = 0; j < J; ++j) {
	    double coef_j = empty ? 1 : coef[j];
	    if (coef_j > 0) {
		tau_shape += 0.5;
		tau_rate += coef_j * scaledY[j] * scaledY[j] / 2;
	    }
	}
	if (coef != _coef) {
	    delete [] coef;
	    coef = 0;
	}

	// Sample from the posterior
	_tau = rgamma(tau_shape, 1/tau_rate, rng);

	/* 
	   Update _scale, which has a conjugate normal distribution given
	   Y and scaledY (Note that it is independent of _tau)
	*/
	vector<double> Yold(J); //Current value of Y
	vector<double> Ymean(J); //Prior mean of Y
	for (unsigned int j = 0; j < J; ++j) {
	    Yold[j] = getValue(Ynodes[j], _chain);
	    Ymean[j] = getMean(Ynodes[j], _chain);
	}

	vector<StochasticNode const *> const &Znodes = _gv2->stochasticChildren();
	unsigned int I = Znodes.size();

	/* 
	   Zprecision[i] is the precision of Z[i]
	   Zresidual[i] is the difference between Z[i] and its prior mean
	   when scaledY==0
	   Zlp[i] is the contribution of scaledY to the linear predictor
	   (excluding intercept terms)
	*/
	vector<double> Zlp(I), Zresidual(I), Zprecision(I);
	for (unsigned int i = 0; i < I; ++i) {
	    Zresidual[i] = *Znodes[i]->value(_chain);
	    Zlp[i] = getMean(Znodes[i], _chain);
	    Zprecision[i] = getScale(Znodes[i], _chain);
	}
	_gv2->setValue(Ymean, _chain);
	for (unsigned int i = 0; i < I; ++i) {
	    Zresidual[i] -= getMean(Znodes[i], _chain);
	    Zlp[i] -= getMean(Znodes[i], _chain);
	    Zlp[i] /= _scale; 
	}
	_gv2->setValue(Yold, _chain);

	double mean_scale = 0, precision_scale=1;
	for (unsigned int i = 0; i < I; ++i) {
	    precision_scale += Zlp[i] * Zlp[i] * Zprecision[i];
	    mean_scale += Zresidual[i] * Zprecision[i] * Zlp[i];
	}
	mean_scale /= precision_scale;

	for(unsigned int r = 0; r <= MAX_RNORM; r++) {
	    if (r == MAX_RNORM) {
		throwRuntimeError("Degenerate scale in Conjugate F Sampler");
	    }
	    _scale = rnorm(mean_scale, 1/sqrt(precision_scale), rng);
	    if (fabs(_scale) > MIN_SCALE) {
		break;
	    }
	}

	double xnew = _tau/(_scale * _scale);
	//std::cout << _tau << " " << _scale << " " << xnew << std::endl;
	_gv1->setValue(&xnew, 1, _chain);
    }

}

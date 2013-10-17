/* 
 * Adapted from R code published in conjunction with:
 * 
 * Liao, J.G. And Rosen, O. (2001) Fast and Stable Algorithms for
 * Computing and Sampling from the Noncentral Hypergeometric
 * Distribution.  The American Statistician 55, 366-369.
 */

#include <config.h>
#include "DHyper.h"
#include <rng/RNG.h>
#include <util/nainf.h>

#include <algorithm>
#include <vector>
#include <stdexcept>

#include <JRmath.h>

using std::vector;
using std::max;
using std::min;
using std::vector;
using std::logic_error;

namespace jags {
namespace bugs {

DHyper::DHyper()
    : RScalarDist("dhyper", 4, DIST_SPECIAL, true)
{}

bool DHyper::canBound() const
{
    return false;
}

static void
getParameters(int &n1, int &n2, int &m1, double &psi,
	      vector<double const *> const &parameters)
{
    n1 = static_cast<int>(*parameters[0]);
    n2 = static_cast<int>(*parameters[1]);
    m1 = static_cast<int>(*parameters[2]);
    psi = *parameters[3];
}

bool 
DHyper::checkParameterDiscrete (vector<bool> const &mask) const
{
    // Check that n1, n2, m1 are discrete-valued
    for (unsigned int i = 0; i < 3; ++i) {
	if (mask[i] == false)
	    return false;
    }
 
    return true;
}

bool DHyper::checkParameterValue (vector<double const *> const &params) const
{
    int n1,n2,m1;
    double psi;
    getParameters(n1, n2, m1, psi, params);

    if (n1 < 0 || n2 < 0)
	return false;
    else if (m1 < 0 || m1 > n1 + n2)
	return false;
    else if (psi <= 0)
	return false;
    else
	return true;
}

static int modeCompute(int n1, int n2, int m1, double psi)
{
    double a =  psi - 1;
    double b =  -((m1 + n1 + 2) * psi + n2 - m1);
    double c = psi * (n1 + 1) * (m1 + 1);
    double q = b;
    if (b > 0) {
	q += sqrt(b * b - 4 * a * c);
    }
    else {
	q -= sqrt(b * b - 4 * a * c);
    }
    q = -q/2;
    int mode = static_cast<int>(c/q);
    if (mode >= 0 && mode >= m1 - n2 && mode <= n1 && mode <= m1) {
	return mode;
    }
    else {
	return static_cast<int>(q/a);
    }
}

double rfunction(int n1, int n2, int m1, double psi, int i) 
{
    return psi * (n1 - i + 1) * (m1 - i + 1)/(i * (n2 - m1 + i));
}  

static vector<double> density(int n1, int n2, int m1, double psi)
{
    int ll = max((int) 0, m1 - n2);
    int uu = min(n1, m1);
    int N = uu - ll + 1;
    vector<double> p(N, 1);

    int mode = modeCompute(n1, n2, m1, psi);

    // Calculate density above the mode
    if (mode < uu) {
	double r = 1;
	for (int i = mode + 1; i <= uu; ++i) {
	    r *= rfunction(n1, n2, m1, psi, i);
	    p[i - ll] = r;
	}
    }
    // Calculate density below the node
    if (mode > ll) {
	double r = 1;
	for (int i = mode; i > ll; --i) {
	    r /= rfunction(n1, n2, m1, psi, i);
	    p[i - ll - 1] = r;
	}
    }
  
    //Normalize
    double sump = 0;
    for (int i = 0; i < N; ++i) {
	sump += p[i];
    }
    for (int i = 0; i < N; ++i) {
	p[i] /= sump;
    }
    return p;
}

static int 
sampleLowToHigh(int lower_end, double ran, vector<double> const &pi)
{
    int upper = pi.size() - 1;
    if (lower_end < 0 || lower_end > upper) {
	throw logic_error("Internal error in Hypergeometric distribution");
    }
    for (int i = lower_end; i < upper; ++i) {
	ran -= pi[i];
	if (ran <= 0) return i;
    }
    return upper;
}

static int 
sampleHighToLow(int upper_end, double ran, vector<double> const &pi)
{
    if (upper_end < 0 || upper_end >= pi.size()) {
	throw logic_error("Internal error in Hypergeometric distribution");
    }

    for (int i = upper_end; i > 0; --i) {
	ran -= pi[i];
	if (ran <= 0) return i;
    }
    return 0;
}

static int singleDraw(int mode, vector<double> const &pi, double ran) 
{
    if (mode == 0) return sampleLowToHigh(0, ran, pi);
    int max = pi.size() - 1;
    if (mode == max) return sampleHighToLow(max, ran, pi);

    ran -= pi[mode];
    if (ran < 0) return mode;

    int lower = mode - 1;
    int upper = mode + 1;
    while (true) {
	if (pi[upper] >= pi[lower]) {
	    ran -= pi[upper];
	    if (ran < 0) return upper;
	    else if (upper == max) return sampleHighToLow(lower, ran, pi);
	    ++upper;
	}
	else {
	    ran -= pi[lower];
	    if (ran < 0) return lower;
	    else if (lower == 0) return sampleLowToHigh(upper, ran, pi);
	    --lower;
	}
    }
}

double DHyper::d(double z, PDFType type,
                 vector<double const *> const &parameters, 
		 bool give_log) const
{
    int n1,n2,m1;
    double psi;
    getParameters(n1, n2, m1, psi, parameters);

    int x = static_cast<int>(z);
    int ll = max((int) 0, m1 - n2);
    int uu = min(n1, m1);

    double den = 0;
    if (x >= ll && x <= uu) {
	den = density(n1, n2, m1, psi)[x - ll];
    }

    if (give_log) {
	return den == 0 ? JAGS_NEGINF : log(den);
    }
    else {
	return den;
    }
}

double DHyper::p(double x, vector<double const *> const &parameters, bool lower,
	         bool give_log) const
{
    int n1,n2,m1;
    double psi;
    getParameters(n1, n2, m1, psi, parameters);

    int ll = max((int) 0, m1 - n2);
    int uu = min(n1, m1);

    double sumpi = 0;
    if (x >= ll) {
	if (x >= uu) {
	    sumpi = 1;
	}
	else {
	    vector<double> pi = density(n1, n2, m1, psi);
	    for (int i = ll; i <= x; ++i) {
		sumpi += pi[i - ll];
	    }
	}
    }

    if (!lower)
	sumpi = max(1 - sumpi, 0.0);
  
    if (give_log) {
	return sumpi == 0 ? JAGS_NEGINF : log(sumpi);
    }
    else {
	return sumpi;
    }
}

double DHyper::q(double p, vector<double const *> const &parameters, bool lower,
               bool log_p) const
{
    int n1,n2,m1;
    double psi;
    getParameters(n1, n2, m1, psi, parameters);
  
    int ll = max((int) 0, m1 - n2);
    int uu = min(n1, m1);
  
    vector<double> pi = density(n1, n2, m1, psi);

    if (log_p)
	p = exp(p);
    if (!lower)
	p = 1 - p;

    double sumpi = 0;
    for (int i = ll; i < uu; ++i) {
	sumpi += pi[i - ll];
	if (sumpi >= p) {
	    return i;
	}
    }
    return uu;
}

double DHyper::r(vector<double const *> const &parameters, RNG *rng) const
{
    int n1,n2,m1;
    double psi;
    getParameters(n1, n2, m1, psi, parameters);

    int mode = modeCompute(n1, n2, m1, psi);
    vector<double> pi = density(n1, n2, m1, psi);
    return singleDraw(mode, pi, rng->uniform());
}

double DHyper::l(vector<double const *> const &parameters) const
{
    int n1,n2,m1;
    double psi;
    getParameters(n1, n2, m1, psi, parameters);
  
    return max((int) 0, m1 - n2);
}

double DHyper::u(vector<double const *> const &parameters) const
{
    int n1,n2,m1;
    double psi;
    getParameters(n1, n2, m1, psi, parameters);
  
    return min(n1, m1);
}

bool DHyper::isSupportFixed(vector<bool> const &fixmask) const
{
    return fixmask[0] && fixmask[1] && fixmask[2]; //Margins fixed
}

double DHyper::KL(vector<double const *> const &para,
		  vector<double const *> const &parb) const
{
    int n1a,n2a,m1a;
    double psia;
    getParameters(n1a, n2a, m1a, psia, para);

    int lla = max((int) 0, m1a - n2a);
    int uua = min(n1a, m1a);

    int n1b,n2b,m1b;
    double psib;
    getParameters(n1b, n2b, m1b, psib, para);

    int llb = max((int) 0, m1b - n2b);
    int uub = min(n1b, m1b);

    if (lla < llb || uua > uub)
	return JAGS_POSINF;

    vector<double> da = density(n1a, n2a, m1a, psia);
    vector<double> db = density(n1b, n2b, m1b, psib);

    double y = 0;
    for (int i = lla; i <= uua; ++i) {
	double proba = da[i - lla];
	double probb = db[i - llb];
	y += proba * (log(proba) - log(probb));
    }
    return y;
}

}}

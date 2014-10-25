#include "testbugsdist.h"

#include "DBern.h"
#include "DBeta.h"
#include "DBin.h"
#include "DCat.h"
#include "DChisqr.h"
#include "DDexp.h"
#include "DDirch.h"
#include "DExp.h"
#include "DF.h"
#include "DGamma.h"
#include "DGenGamma.h"
#include "DHyper.h"
#include "DInterval.h"
#include "DLnorm.h"
#include "DLogis.h"
#include "DMNorm.h"
#include "DMT.h"
#include "DMulti.h"
#include "DNChisqr.h"
#include "DNegBin.h"
#include "DNorm.h"
#include "DPar.h"
#include "DPois.h"
#include "DRound.h"
#include "DSum.h"
#include "DT.h"
#include "DUnif.h"
#include "DWeib.h"
#include "DWish.h"

#include <MersenneTwisterRNG.h>

using std::string;
using std::vector;

using jags::RScalarDist;

void BugsDistTest::setUp() {

    _rng = new jags::base::MersenneTwisterRNG(1234567, 
					      jags::KINDERMAN_RAMAGE);

    _dbern = new jags::bugs::DBern();
    _dbeta = new jags::bugs::DBeta();
    _dbin = new jags::bugs::DBin();
    _dcat = new jags::bugs::DCat();
    _dchisqr = new jags::bugs::DChisqr();
    _ddirch = new jags::bugs::DDirch();
    _dexp = new jags::bugs::DExp();
    _df = new jags::bugs::DF();
    _dgamma = new jags::bugs::DGamma();
    _dgengamma = new jags::bugs::DGenGamma();
    _dhyper = new jags::bugs::DHyper();
    _dnterval = new jags::bugs::DInterval();    
    _dlnorm = new jags::bugs::DLnorm();
    _dlogis = new jags::bugs::DLogis();
    _dmnorm = new jags::bugs::DMNorm();
    _dmt = new jags::bugs::DMT();
    _dmulti = new jags::bugs::DMulti();
    _dnchisqr = new jags::bugs::DNChisqr();
    _dnegbin = new jags::bugs::DNegBin();
    _dnorm = new jags::bugs::DNorm();
    _dpar = new jags::bugs::DPar();
    _dpois = new jags::bugs::DPois();
    _dround = new jags::bugs::DRound();
    _dsum = new jags::bugs::DSum();
    _dt = new jags::bugs::DT();
    _dunif = new jags::bugs::DUnif();
    _dweib = new jags::bugs::DWeib();
    _dwish = new jags::bugs::DWish();

}

void BugsDistTest::tearDown() {

    delete _rng;

    delete _dbern;
    delete _dbeta;
    delete _dbin;
    delete _dcat;
    delete _dchisqr;
    delete _ddirch;
    delete _dexp;
    delete _df;
    delete _dgamma;
    delete _dgengamma;
    delete _dhyper;
    delete _dnterval;    
    delete _dlnorm;
    delete _dlogis;
    delete _dmnorm;
    delete _dmt;
    delete _dmulti;
    delete _dnchisqr;
    delete _dnegbin;
    delete _dnorm;
    delete _dpar;
    delete _dpois;
    delete _dround;
    delete _dsum;
    delete _dt;
    delete _dunif;
    delete _dweib;
    delete _dwish;

}

void BugsDistTest::npar()
{
    //CPPUNIT_ASSERT_MESSAGE("npar check", false);

    CPPUNIT_ASSERT_EQUAL(_dbern->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_dbeta->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dbin->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dcat->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_dchisqr->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_ddirch->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_dexp->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_df->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dgamma->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dgengamma->npar(), 3U);
    CPPUNIT_ASSERT_EQUAL(_dhyper->npar(), 4U);
    CPPUNIT_ASSERT_EQUAL(_dnterval->npar(), 2U);    
    CPPUNIT_ASSERT_EQUAL(_dlnorm->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dlogis->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dmnorm->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dmt->npar(), 3U);
    CPPUNIT_ASSERT_EQUAL(_dmulti->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dchisqr->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_dnegbin->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dnorm->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dpar->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dpois->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_dround->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dsum->npar(), 0U);
    CPPUNIT_ASSERT_EQUAL(_dt->npar(), 3U);
    CPPUNIT_ASSERT_EQUAL(_dunif->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dweib->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dwish->npar(), 2U);

}

void BugsDistTest::name()
{
    CPPUNIT_ASSERT_EQUAL(string("dbern"), _dbern->name());
    CPPUNIT_ASSERT_EQUAL(string("dbeta"), _dbeta->name());
    CPPUNIT_ASSERT_EQUAL(string("dbin"), _dbin->name());
    CPPUNIT_ASSERT_EQUAL(string("dcat"), _dcat->name());
    CPPUNIT_ASSERT_EQUAL(string("dchisqr"), _dchisqr->name());
    CPPUNIT_ASSERT_EQUAL(string("ddirch"), _ddirch->name());
    CPPUNIT_ASSERT_EQUAL(string("dexp"), _dexp->name());
    CPPUNIT_ASSERT_EQUAL(string("df"), _df->name());
    CPPUNIT_ASSERT_EQUAL(string("dgamma"), _dgamma->name());
    CPPUNIT_ASSERT_EQUAL(string("dgen.gamma"), _dgengamma->name());
    CPPUNIT_ASSERT_EQUAL(string("dhyper"), _dhyper->name());
    CPPUNIT_ASSERT_EQUAL(string("dinterval"), _dnterval->name());    
    CPPUNIT_ASSERT_EQUAL(string("dlnorm"), _dlnorm->name());
    CPPUNIT_ASSERT_EQUAL(string("dlogis"), _dlogis->name());
    CPPUNIT_ASSERT_EQUAL(string("dmnorm"), _dmnorm->name());
    CPPUNIT_ASSERT_EQUAL(string("dmt"), _dmt->name());
    CPPUNIT_ASSERT_EQUAL(string("dmulti"), _dmulti->name());
    CPPUNIT_ASSERT_EQUAL(string("dnchisqr"), _dnchisqr->name());
    CPPUNIT_ASSERT_EQUAL(string("dnegbin"), _dnegbin->name());
    CPPUNIT_ASSERT_EQUAL(string("dnorm"), _dnorm->name());
    CPPUNIT_ASSERT_EQUAL(string("dpar"), _dpar->name());
    CPPUNIT_ASSERT_EQUAL(string("dpois"), _dpois->name());
    CPPUNIT_ASSERT_EQUAL(string("dround"), _dround->name());
    CPPUNIT_ASSERT_EQUAL(string("dsum"), _dsum->name());
    CPPUNIT_ASSERT_EQUAL(string("dt"), _dt->name());
    CPPUNIT_ASSERT_EQUAL(string("dunif"), _dunif->name());
    CPPUNIT_ASSERT_EQUAL(string("dweib"), _dweib->name());
    CPPUNIT_ASSERT_EQUAL(string("dwish"), _dwish->name());

}

void BugsDistTest::alias()
{
    CPPUNIT_ASSERT_EQUAL(string(""), _dbern->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dbeta->name());
    CPPUNIT_ASSERT_EQUAL(string("dbinom"), _dbin->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dcat->name());
    CPPUNIT_ASSERT_EQUAL(string("dchisq"), _dchisqr->name());
    CPPUNIT_ASSERT_EQUAL(string("ddirich"), _ddirch->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dexp->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _df->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dgamma->name());
    CPPUNIT_ASSERT_EQUAL(string("dggamma"), _dgengamma->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dhyper->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dnterval->name());    
    CPPUNIT_ASSERT_EQUAL(string(""), _dlnorm->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dlogis->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dmnorm->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dmt->name());
    CPPUNIT_ASSERT_EQUAL(string("dmultinom"), _dmulti->name());
    CPPUNIT_ASSERT_EQUAL(string("dnchisq"), _dnchisqr->name());
    CPPUNIT_ASSERT_EQUAL(string("dnbinom"), _dnegbin->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dnorm->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dpar->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dpois->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dround->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dsum->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dt->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dunif->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dweib->name());
    CPPUNIT_ASSERT_EQUAL(string(""), _dwish->name());
}

void BugsDistTest::rscalar_rpq(RScalarDist const *dist, 
			       vector<double const *> const &par)
{
    /*
      Simultaneous test of r, p, and q functions for distributions
      inheriting from RScalarDist.
    */
    unsigned int nsim = 100;

    CPPUNIT_ASSERT_MESSAGE(dist->name(), checkNPar(dist, par.size()));
    CPPUNIT_ASSERT_MESSAGE(dist->name(), dist->checkParameterValue(par));

    for (unsigned int i = 0; i < nsim; ++i) {
	//Generate random variable from distribution
	double y = dist->r(par, _rng);
	//Pass to distribution function and then to distribution function
	double p = dist->p(y, par, true, false);
	CPPUNIT_ASSERT_MESSAGE(dist->name(), p >= 0 && p <= 1);
	double z = dist->q(p, par, true, false);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(dist->name(), y, z, tol);
	//Now do the same on a log scale
	double logp = dist->p(y, par, true, true);
	CPPUNIT_ASSERT_MESSAGE(dist->name(), logp <= 0);
	z = dist->q(logp, par, true, true);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(dist->name(), y, z, tol);
	//Using upper tail
	p = dist->p(y, par, false, false);
	CPPUNIT_ASSERT_MESSAGE(dist->name(), p >= 0 && p <= 1);
	z = dist->q(p, par, false, false);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(dist->name(), y, z, tol);
	//Upper tail on log scale
	logp = dist->p(y, par, false, true);
	CPPUNIT_ASSERT_MESSAGE(dist->name(), logp <= 0);
	z = dist->q(logp, par, false, true);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(dist->name(), y, z, tol);
    }
}

static vector<double const *> mkPar(double const &v1)
{
    vector<double const *> par;
    par.push_back(&v1);
    return par;
}

static vector<double const *> mkPar(double const &v1, double const &v2)
{
    vector<double const *> par;
    par.push_back(&v1);
    par.push_back(&v2);
    return par;
}

static vector<double const *> mkPar(double const &v1, double const &v2,
				    double const &v3)
{
    vector<double const *> par;
    par.push_back(&v1);
    par.push_back(&v2);
    par.push_back(&v3);
    return par;
}

static vector<double const *> mkPar(double const &v1, double const &v2,
				    double const &v3, double const &v4)
{
    vector<double const *> par;
    par.push_back(&v1);
    par.push_back(&v2);
    par.push_back(&v3);
    par.push_back(&v4);
    return par;
}

void BugsDistTest::rscalar()
{
    rscalar_rpq(_dbeta, mkPar(0.3, 0.5));
    rscalar_rpq(_dbeta, mkPar(2, 6));
    
    rscalar_rpq(_dbin, mkPar(0.1, 10));
    rscalar_rpq(_dbin, mkPar(0.9, 1));
    
    rscalar_rpq(_dchisqr, mkPar(1));
    rscalar_rpq(_dchisqr, mkPar(2));
    rscalar_rpq(_dchisqr, mkPar(3));
    rscalar_rpq(_dchisqr, mkPar(10));

    rscalar_rpq(_dexp, mkPar(0.1));
    rscalar_rpq(_dexp, mkPar(7));

    rscalar_rpq(_df, mkPar(1, 1));
    rscalar_rpq(_df, mkPar(3, 7));

    rscalar_rpq(_dgamma, mkPar(1, 0.1));
    rscalar_rpq(_dgamma, mkPar(0.1, 3));

    rscalar_rpq(_dgengamma, mkPar(1, 0.1, 3));
    rscalar_rpq(_dgengamma, mkPar(0.1, 3, 0.5));

    for (int i = 1; i <= 10; ++i) {
	for (int j = 0; j <= i; ++j ) {
	    rscalar_rpq(_dhyper, mkPar(i, i, j, 1));
	    rscalar_rpq(_dhyper, mkPar(i, i, j, 0.5));
	    rscalar_rpq(_dhyper, mkPar(i, i, j, 3));
	}
    }
    rscalar_rpq(_dhyper, mkPar(10, 8, 7, 0.5));

    rscalar_rpq(_dlnorm, mkPar(-0.5, 33));
    rscalar_rpq(_dlogis, mkPar(-7, 0.3));
    rscalar_rpq(_dnchisqr, mkPar(5, 1.5));
    rscalar_rpq(_dnorm, mkPar(3, 10));
    rscalar_rpq(_dpar, mkPar(1.3, 1));

    rscalar_rpq(_dpois, mkPar(0.12));
    rscalar_rpq(_dpois, mkPar(10.7));
    rscalar_rpq(_dt, mkPar(18, 1.7, 1));
    rscalar_rpq(_dt, mkPar(0, 1, 4));
    rscalar_rpq(_dweib, mkPar(3, 10));
    rscalar_rpq(_dweib, mkPar(0.9, 0.9));

}

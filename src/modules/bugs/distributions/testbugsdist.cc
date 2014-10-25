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

void BugsDistTest::setUp() {

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

}

void BugsDistTest::alias()
{

}

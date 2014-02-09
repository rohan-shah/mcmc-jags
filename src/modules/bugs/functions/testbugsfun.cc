#include "testbugsfun.h"

#include "Abs.h"
#include "ArcCos.h"
#include "ArcCosh.h"
#include "ArcSin.h"
#include "ArcSinh.h"
#include "ArcTan.h"
#include "ArcTanh.h"
#include "CLogLog.h"
#include "Cos.h"
#include "Cosh.h"
#include "DIntervalFunc.h"
#include "DRoundFunc.h"
#include "DSumFunc.h"
#include "Equals.h"
#include "Exp.h"
#include "ICLogLog.h"
#include "IfElse.h"
#include "ILogit.h"
#include "InProd.h"
#include "InterpLin.h"
#include "Inverse.h"
#include "LogDet.h"
#include "LogFact.h"
#include "LogGam.h"
#include "Log.h"
#include "Logit.h"
#include "MatMult.h"
#include "Max.h"
#include "Mean.h"
#include "Min.h"
#include "Order.h"
#include "Phi.h"
#include "Probit.h"
#include "Prod.h"
#include "Rank.h"
#include "Round.h"
#include "SD.h"
#include "Sin.h"
#include "Sinh.h"
#include "Sort.h"
#include "Sqrt.h"
#include "Step.h"
#include "Sum.h"
#include "Tan.h"
#include "Tanh.h"
#include "Transpose.h"
#include "Trunc.h"

#include <JRmath.h>

#include <function/testfun.h>
#include <util/nainf.h>
#include <util/integer.h>

using jags::checkInteger;
using jags::LinkFunction;
using jags::ScalarFunction;
using jags::VectorFunction;
using jags::Function;

using std::vector;
using std::string;

#include <climits>
#include <cmath>

void BugsFunTest::setUp()
{
    //Trigonometric functions and their inverses
    _sin = new jags::bugs::Sin;
    _cos = new jags::bugs::Cos;
    _tan = new jags::bugs::Tan;
    _arccos = new jags::bugs::ArcCos;
    _arcsin = new jags::bugs::ArcSin;
    _arctan = new jags::bugs::ArcTan;

    //Hyperbolic functions and their inverses
    _sinh = new jags::bugs::Sinh;
    _cosh = new jags::bugs::Cosh;
    _tanh = new jags::bugs::Tanh;
    _arcsinh = new jags::bugs::ArcSinh;
    _arccosh = new jags::bugs::ArcCosh;
    _arctanh = new jags::bugs::ArcTanh;

    //Link functions and their inverses
    _cloglog = new jags::bugs::CLogLog;
    _log = new jags::bugs::Log;
    _logit = new jags::bugs::Logit;
    _probit = new jags::bugs::Probit;
    _icloglog = new jags::bugs::ICLogLog;
    _exp = new jags::bugs::Exp;
    _ilogit = new jags::bugs::ILogit;
    _phi = new jags::bugs::Phi;

    //Scalar summaries of vectors
    _max = new jags::bugs::Max;
    _mean = new jags::bugs::Mean;
    _min = new jags::bugs::Min;
    _sum = new jags::bugs::Sum;
    _sd = new jags::bugs::SD;
    _prod = new jags::bugs::Prod;

    //Mathematical functions
    _logfact = new jags::bugs::LogFact;
    _loggam = new jags::bugs::LogGam;
    _sqrt = new jags::bugs::Sqrt;

    //Lossy scalar functions
    _round = new jags::bugs::Round;
    _step = new jags::bugs::Step;
    _trunc = new jags::bugs::Trunc;
    _abs = new jags::bugs::Abs;

    //Observable functions
    _dinterval = new jags::bugs::DIntervalFunc;
    _dround = new jags::bugs::DRoundFunc;
    _dsum = new jags::bugs::DSumFunc;

//_equals = new jags::bugs::Equals;

    //Sorting functions
    _order = new jags::bugs::Order;
    _rank = new jags::bugs::Rank;
    _sort = new jags::bugs::Sort;

    //Matrix functions
    _inverse = new jags::bugs::Inverse;
    _logdet = new jags::bugs::LogDet;
    _matmult = new jags::bugs::MatMult;
    _transpose = new jags::bugs::Transpose;

    //Odds and sods
    _ifelse = new jags::bugs::IfElse;
    _inprod = new jags::bugs::InProd;
    _interplin = new jags::bugs::InterpLin;
}

void BugsFunTest::tearDown()
{
    //Trigonometric functions and their inverses
    delete _sin;
    delete _cos;
    delete _tan;
    delete _arccos;
    delete _arcsin;
    delete _arctan;

    //Hyperbolic functions and their inverses
    delete _sinh;
    delete _cosh;
    delete _tanh;
    delete _arcsinh;
    delete _arccosh;
    delete _arctanh;

    //Link functions and their inverses
    delete _cloglog;
    delete _log;
    delete _logit;
    delete _probit;
    delete _icloglog;
    delete _exp;
    delete _ilogit;
    delete _phi;

    //Scalar summaries of vectors
    delete _max;
    delete _mean;
    delete _min;
    delete _sum;
    delete _sd;
    delete _prod;

    //Mathematical functions
    delete _logfact;
    delete _loggam;
    delete _sqrt;

    //Lossy scalar functions
    delete _round;
    delete _step;
    delete _trunc;
    delete _abs;

    //Observable functions
    delete _dinterval;
    delete _dround;
    delete _dsum;

//delete _equals 

    //Sorting functions
    delete _order;
    delete _rank;
    delete _sort;

    //Matrix functions
    delete _inverse;
    delete _logdet;
    delete _matmult;
    delete _transpose;
    delete _inprod;

    //Odds and sods
    delete _ifelse;
    delete _interplin;
}

void BugsFunTest::npar()
{
    //Trigonometric functions and their inverses
    CPPUNIT_ASSERT_EQUAL(_sin->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_cos->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_tan->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_arccos->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_arcsin->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_arctan->npar(), 1U);

    //Hyperbolic functions and their inverses
    CPPUNIT_ASSERT_EQUAL(_sinh->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_cosh->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_tanh->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_arcsinh->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_arccosh->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_arctanh->npar(), 1U);

    //Link functions and their inverses
    CPPUNIT_ASSERT_EQUAL(_cloglog->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_log->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_logit->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_probit->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_icloglog->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_exp->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_ilogit->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_phi->npar(), 1U);

    //Scalar summaries of vectors
    CPPUNIT_ASSERT_EQUAL(_max->npar(), 0U);
    CPPUNIT_ASSERT_EQUAL(_min->npar(), 0U);
    CPPUNIT_ASSERT_EQUAL(_sum->npar(), 0U);
    CPPUNIT_ASSERT_EQUAL(_prod->npar(), 0U);
    CPPUNIT_ASSERT_EQUAL(_sd->npar(), 1U);

    //Mathematical functions
    CPPUNIT_ASSERT_EQUAL(_logfact->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_loggam->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_sqrt->npar(), 1U);

    //Lossy scalar functions
    CPPUNIT_ASSERT_EQUAL(_round->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_step->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_trunc->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_abs->npar(), 1U);

    //Observable functions
    CPPUNIT_ASSERT_EQUAL(_dinterval->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_dround->npar(), 2U);
    CPPUNIT_ASSERT(checkNPar(_dsum, 1));

    //Sorting functions
    CPPUNIT_ASSERT_EQUAL(_order->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_rank->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_sort->npar(), 1U);

    //Matrix functions
    CPPUNIT_ASSERT_EQUAL(_inverse->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_logdet->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_matmult->npar(), 2U);
    CPPUNIT_ASSERT_EQUAL(_transpose->npar(), 1U);
    CPPUNIT_ASSERT_EQUAL(_inprod->npar(), 2U);

    //Odds and sods
    CPPUNIT_ASSERT_EQUAL(_ifelse->npar(), 3U);
    CPPUNIT_ASSERT_EQUAL(_interplin->npar(), 3U);
}

void BugsFunTest::name()
{
    //Trigonometric functions
    CPPUNIT_ASSERT_EQUAL(string("sin"), _sin->name());
    CPPUNIT_ASSERT_EQUAL(string("cos"), _cos->name());
    CPPUNIT_ASSERT_EQUAL(string("tan"), _tan->name());
    CPPUNIT_ASSERT_EQUAL(string("arcsin"), _arcsin->name());
    CPPUNIT_ASSERT_EQUAL(string("arccos"), _arccos->name());
    CPPUNIT_ASSERT_EQUAL(string("arctan"), _arctan->name());

    //Hyperbolic functions
    CPPUNIT_ASSERT_EQUAL(string("sinh"), _sinh->name());
    CPPUNIT_ASSERT_EQUAL(string("cosh"), _cosh->name());
    CPPUNIT_ASSERT_EQUAL(string("tanh"), _tanh->name());
    CPPUNIT_ASSERT_EQUAL(string("arcsinh"), _arcsinh->name());
    CPPUNIT_ASSERT_EQUAL(string("arccosh"), _arccosh->name());
    CPPUNIT_ASSERT_EQUAL(string("arctanh"), _arctanh->name());

    //Link functions
    CPPUNIT_ASSERT_EQUAL(string("cloglog"), _cloglog->name());
    CPPUNIT_ASSERT_EQUAL(string("log"), _log->name());
    CPPUNIT_ASSERT_EQUAL(string("logit"), _logit->name());
    CPPUNIT_ASSERT_EQUAL(string("probit"), _probit->name());
    CPPUNIT_ASSERT_EQUAL(string("icloglog"), _icloglog->name());
    CPPUNIT_ASSERT_EQUAL(string("exp"), _exp->name());
    CPPUNIT_ASSERT_EQUAL(string("ilogit"), _ilogit->name());
    CPPUNIT_ASSERT_EQUAL(string("phi"), _phi->name());

    //Scalar summaries of vectors
    CPPUNIT_ASSERT_EQUAL(string("max"), _max->name());
    CPPUNIT_ASSERT_EQUAL(string("mean"), _mean->name());
    CPPUNIT_ASSERT_EQUAL(string("min"), _min->name());
    CPPUNIT_ASSERT_EQUAL(string("sum"), _sum->name());
    CPPUNIT_ASSERT_EQUAL(string("sd"), _sd->name());
    CPPUNIT_ASSERT_EQUAL(string("prod"), _prod->name());

    //Mathematical functions
    CPPUNIT_ASSERT_EQUAL(string("logfact"), _logfact->name());
    CPPUNIT_ASSERT_EQUAL(string("loggam"), _loggam->name());
    CPPUNIT_ASSERT_EQUAL(string("sqrt"), _sqrt->name());

    //Lossy scalar functions
    CPPUNIT_ASSERT_EQUAL(string("round"), _round->name());
    CPPUNIT_ASSERT_EQUAL(string("step"), _step->name());
    CPPUNIT_ASSERT_EQUAL(string("trunc"), _trunc->name());
    CPPUNIT_ASSERT_EQUAL(string("abs"), _abs->name());

    //Observable functions
    CPPUNIT_ASSERT_EQUAL(string("dinterval"), _dinterval->name());
    CPPUNIT_ASSERT_EQUAL(string("dround"), _dround->name());
    CPPUNIT_ASSERT_EQUAL(string("dsum"), _dsum->name());

    CPPUNIT_ASSERT_EQUAL(string("order"), _order->name());
    CPPUNIT_ASSERT_EQUAL(string("rank"), _rank->name());
    CPPUNIT_ASSERT_EQUAL(string("sort"), _sort->name());

    //Matrix functions
    CPPUNIT_ASSERT_EQUAL(string("inverse"), _inverse->name());
    CPPUNIT_ASSERT_EQUAL(string("logdet"), _logdet->name());
    CPPUNIT_ASSERT_EQUAL(string("%*%"), _matmult->name());
    CPPUNIT_ASSERT_EQUAL(string("t"), _transpose->name());

    //odds and ends
    CPPUNIT_ASSERT_EQUAL(string("ifelse"), _ifelse->name());
    CPPUNIT_ASSERT_EQUAL(string("inprod"), _inprod->name());
    CPPUNIT_ASSERT_EQUAL(string("interp.lin"), _interplin->name());
}

void BugsFunTest::alias()
{
    //Trigonometric functions
    CPPUNIT_ASSERT_EQUAL(string(""), _sin->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _cos->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _tan->alias());
    CPPUNIT_ASSERT_EQUAL(string("asin"), _arcsin->alias());
    CPPUNIT_ASSERT_EQUAL(string("acos"), _arccos->alias());
    CPPUNIT_ASSERT_EQUAL(string("atan"), _arctan->alias());

    //Hyperbolic functions
    CPPUNIT_ASSERT_EQUAL(string(""), _sinh->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _cosh->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _tanh->alias());
    CPPUNIT_ASSERT_EQUAL(string("asinh"), _arcsinh->alias());
    CPPUNIT_ASSERT_EQUAL(string("acosh"), _arccosh->alias());
    CPPUNIT_ASSERT_EQUAL(string("atanh"), _arctanh->alias());

    //Link functions
    CPPUNIT_ASSERT_EQUAL(string(""), _cloglog->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _log->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _logit->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _probit->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _icloglog->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _exp->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _ilogit->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _phi->alias());

    //Scalar summaries of vectors
    CPPUNIT_ASSERT_EQUAL(string(""), _max->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _mean->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _min->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _sum->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _sd->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _prod->alias());

    //Mathematical functions
    CPPUNIT_ASSERT_EQUAL(string(""), _logfact->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _loggam->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _sqrt->alias());

    //Lossy scalar functions
    CPPUNIT_ASSERT_EQUAL(string(""), _round->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _step->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _trunc->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _abs->alias());

    //Observable functions
    CPPUNIT_ASSERT_EQUAL(string(""), _dinterval->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _dround->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _dsum->alias());

    CPPUNIT_ASSERT_EQUAL(string(""), _order->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _rank->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _sort->alias());

    //Matrix functions
    CPPUNIT_ASSERT_EQUAL(string(""), _inverse->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _logdet->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _matmult->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _transpose->alias());

    //odds and ends
    CPPUNIT_ASSERT_EQUAL(string(""), _ifelse->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _inprod->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _interplin->alias());
}

void BugsFunTest::trig(double const v)
{
    double sinv = eval(_sin, v);
    double cosv = eval(_cos, v);
    double tanv = eval(_tan, v);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, sinv*sinv + cosv*cosv, tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tanv, sinv/cosv, tol);

    //Test consistency of inverse functions by inverting twice
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sinv, eval(_sin, eval(_arcsin, sinv)), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(cosv, eval(_cos, eval(_arccos, cosv)), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tanv, eval(_tan, eval(_arctan, tanv)), tol);
}

void BugsFunTest::trig()
{
    //Check reference values
    CPPUNIT_ASSERT_EQUAL(0.0, eval(_sin, 0));
    CPPUNIT_ASSERT_EQUAL(1.0, eval(_cos, 0));
    CPPUNIT_ASSERT_EQUAL(0.0, eval(_tan, 0));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, eval(_sin, M_PI_2), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, eval(_cos, M_PI_2), tol);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, eval(_sin, M_PI), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, eval(_cos, M_PI), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, eval(_tan, M_PI), tol);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, eval(_sin, 3 * M_PI_2), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, eval(_cos, 3 * M_PI_2), tol);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, eval(_sin, M_2PI), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, eval(_cos, M_2PI), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, eval(_tan, M_2PI), tol);

    //Check limits of inverse functions
    checkLimits(_arcsin, -1, 1);
    checkLimits(_arccos, -1, 1);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(-M_PI_2, eval(_arcsin, -1), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( M_PI_2, eval(_arcsin, 1), tol);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(M_PI, eval(_arccos, -1), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0,    eval(_arccos, 1), tol);

    //Test consistency of sin, cos, tan for a range of values
    for (double v = -10; v < 10; v = v + 0.1) {
	trig(v);
    }
}

void BugsFunTest::hyper(const double v)
{
    double sinhv = eval(_sinh, v);
    double coshv = eval(_cosh, v);
    double tanhv = eval(_tanh, v);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, coshv*coshv - sinhv*sinhv, tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(tanhv, sinhv/coshv, tol);

    //Test consistency of inverse functions 
    CPPUNIT_ASSERT_DOUBLES_EQUAL(v, eval(_arcsinh, sinhv), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(fabs(v), eval(_arccosh, coshv), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(v, eval(_arctanh, tanhv), tol);
}

void BugsFunTest::hyper()
{
    //Check reference values
    CPPUNIT_ASSERT_EQUAL(0.0, eval(_sinh, 0));
    CPPUNIT_ASSERT_EQUAL(1.0, eval(_cosh, 0));
    CPPUNIT_ASSERT_EQUAL(0.0, eval(_tanh, 0));

    //Check limits of inverse functions
    checkLimits(_arccosh, 1, JAGS_POSINF);
    checkLimits(_arctanh, -1, 1);

    //Check consistency across a range of values
    for (double v = -5; v < 5; v = v + 0.1) {
	hyper(v);
    }
}

void BugsFunTest::link(ScalarFunction const *f, LinkFunction const *l,
		       double lower, double upper, int N)
{
    //Check that names of f and l match
    CPPUNIT_ASSERT_EQUAL(f->name(), l->linkName());

    //f only takes one argument (no need to check l)
    CPPUNIT_ASSERT_EQUAL(f->npar(), 1U);

    //Test link function over a range of values
    double delta = (upper - lower)/(N - 1);
    double yold;

    for (int i = 0; i < N; ++i) {

	const double x = lower + i * delta;
	const double y = l->inverseLink(x);

	//Inverse link function is strictlying increasing 
	CPPUNIT_ASSERT(l->grad(x) > 0);
	if (i > 0) {
	    CPPUNIT_ASSERT(y > yold);
	}
	yold = y;

	//Inverting the link function puts us back where we were.
	CPPUNIT_ASSERT_DOUBLES_EQUAL(x, eval(f, y), tol);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(x, l->link(y), tol);
    }
}

void BugsFunTest::link()
{
    //Check reference values for link functions
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, _exp->inverseLink(0), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, _ilogit->inverseLink(0), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, _phi->inverseLink(0), tol);

    //Check range of link functions
    checkLimits(_log, 0, JAGS_POSINF);
    checkLimits(_logit, 0, 1);
    checkLimits(_probit, 0, 1);
    checkLimits(_cloglog, 0, 1);

    //Check that the limits of the range map to -Inf, +Inf
    CPPUNIT_ASSERT_EQUAL(JAGS_NEGINF, eval(_log, 0));
    CPPUNIT_ASSERT_EQUAL(JAGS_NEGINF, eval(_logit, 0));
    CPPUNIT_ASSERT_EQUAL(JAGS_NEGINF, eval(_probit, 0));
    CPPUNIT_ASSERT_EQUAL(JAGS_NEGINF, eval(_cloglog, 0));

    CPPUNIT_ASSERT_EQUAL(JAGS_POSINF, eval(_logit, 1));
    CPPUNIT_ASSERT_EQUAL(JAGS_POSINF, eval(_probit, 1));
    CPPUNIT_ASSERT_EQUAL(JAGS_POSINF, eval(_cloglog, 1));

    //Complementary log log link is only invertible up to around 3
    link(_cloglog, _icloglog, -5.0, 3.0, 13);
    //For the rest, we test the range [-5,5]
    link(_log, _exp, -5.0, 5.0, 13);
    link(_logit, _ilogit, -5.0, 5.0, 13);
    link(_probit, _phi, -5.0, 5.0, 13);
}

void BugsFunTest::summary(double const *v, unsigned int N)
{
    //Test scalar summaries of vector values;
    vector<double> arg = mkVec(v, N);

    //Calculate summaries
    double vmax = eval(_max, arg);
    double vmin = eval(_min, arg);
    double vmean = eval(_mean, arg);
    double vsum = eval(_sum, arg);

    //Check consistency of min, max, mean, sum
    CPPUNIT_ASSERT(vmax >= vmean);
    CPPUNIT_ASSERT(vmean >= vmin);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(vsum, vmean * N, tol);

    //Negate argument and recalculate
    vector<double> negarg(N);
    for (unsigned int i = 0; i < N; ++i) {
	negarg[i] = -v[i];
    }

    double negvmax = eval(_max, negarg);
    double negvmin = eval(_min, negarg);
    double negvmean = eval(_mean, negarg);
    double negvsum = eval(_sum, negarg);

    //Check consistency of results for positive and negative args
    CPPUNIT_ASSERT_EQUAL(negvmax, -vmin);
    CPPUNIT_ASSERT_EQUAL(negvmin, -vmax);
    CPPUNIT_ASSERT_EQUAL(negvmean, -vmean);
    CPPUNIT_ASSERT_EQUAL(negvsum, -vsum);

    //Check consistency of sd, sum, mean
    if (N == 1) {
	CPPUNIT_ASSERT(!checkparlen(_sd, N));
    }
    else {
	double vsd = eval(_sd, mkVec(v, N));
	vector<double> v2(N);
	for (unsigned int i = 0; i < N; ++i) v2[i] = v[i]*v[i];
	
	double v2sum = eval(_sum, v2);
	CPPUNIT_ASSERT_DOUBLES_EQUAL((v2sum - vsum*vmean)/(N-1), vsd*vsd, tol);
    }

    //Check consistency of prod and sum on log scale
    vector<double> vexp(N);
    for (unsigned int i = 0; i < N; ++i) {
	vexp[i] = exp(v[i]);
    }
    CPPUNIT_ASSERT_DOUBLES_EQUAL(vsum, log(eval(_prod, vexp)), tol);

}

void BugsFunTest::summary(double const *v1, unsigned int N1,
			  double const *v2, unsigned int N2)
{
    //Test variadic summary functions giving two arguments;
    vector<double> arg1 = mkVec(v1, N1);
    vector<double> arg2 = mkVec(v2, N2);

    CPPUNIT_ASSERT_EQUAL(veval(_max, arg1, arg2).size(), 1UL);
    CPPUNIT_ASSERT_EQUAL(veval(_min, arg1, arg2).size(), 1UL);
    CPPUNIT_ASSERT_EQUAL(veval(_sum, arg1, arg2).size(), 1UL);

    //Calculate summaries
    double vmax = veval(_max, arg1, arg2)[0];
    double vmin = veval(_min, arg1, arg2)[0];
    double vsum = veval(_sum, arg1, arg2)[0];

    //Check consistency of min, max, mean, sum
    CPPUNIT_ASSERT(vmax >= vmin);

    //Check consistency of prod and sum on log scale
    vector<double> v1exp(N1);
    for (unsigned int i = 0; i < N1; ++i) {
	v1exp[i] = exp(v1[i]);
    }
    vector<double> v2exp(N2);
    for (unsigned int i = 0; i < N2; ++i) {
	v2exp[i] = exp(v2[i]);
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL(vsum, log(veval(_prod, v1exp, v2exp)[0]), tol);
}
    
void BugsFunTest::summary()
{
    double v0[1] = {0};
    double v1[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    double v2[5] = {1, 3, 5, 9, 11};
    double v3[2] = {-M_PI, M_PI};
    double v4[6] = {-1, 1, 2, 2, -3, -2.5};

    summary(v0, 1);
    summary(v1, 9);
    summary(v2, 5);
    summary(v3, 2);
    summary(v4, 6);

    summary(v1, 9, v1, 9);
    summary(v1, 9, v2, 5);
    summary(v1, 9, v3, 2);
    summary(v1, 9, v4, 6);

    summary(v2, 5, v2, 5);
    summary(v2, 5, v3, 2);
    summary(v2, 5, v4, 6);

    summary(v3, 2, v3, 2);
    summary(v3, 2, v4, 6);

    summary(v4, 6, v4, 6);
}

void BugsFunTest::math()
{
    CPPUNIT_ASSERT(!checkval(_logfact, -1));
    CPPUNIT_ASSERT(!checkval(_loggam, -1));
    CPPUNIT_ASSERT(!checkval(_sqrt, -1));
    CPPUNIT_ASSERT(!checkval(_loggam, 0));

    CPPUNIT_ASSERT_EQUAL(0.0, eval(_logfact, 0));
    CPPUNIT_ASSERT_EQUAL(0.0, eval(_sqrt, 0));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(log(M_PI)/2, eval(_loggam, 0.5), tol);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, eval(_logfact, 1), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, eval(_loggam, 1), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, eval(_sqrt, 1), tol);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, eval(_loggam, 2), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(log(2), eval(_logfact, 2), tol);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(log(2), eval(_loggam, 3), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(log(6), eval(_logfact, 3), tol);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(log(6), eval(_loggam, 4), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(log(24), eval(_logfact, 4), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, eval(_sqrt, 4), tol);
}

void BugsFunTest::lossy()
{
    //round moves towards the nearest integer
    CPPUNIT_ASSERT_EQUAL(0.0, eval(_round, 0.5 - DBL_EPSILON));
    CPPUNIT_ASSERT_EQUAL(1.0, eval(_round, 0.5 + DBL_EPSILON));
    CPPUNIT_ASSERT_EQUAL(1.0, eval(_round, 1.1));
    CPPUNIT_ASSERT_EQUAL(0.0, eval(_round, -0.4));
    CPPUNIT_ASSERT_EQUAL(-1.0, eval(_round, -0.9));

    //step moves from 0 to 1 at 0
    CPPUNIT_ASSERT_EQUAL(0.0, eval(_step, -DBL_EPSILON));
    CPPUNIT_ASSERT_EQUAL(1.0, eval(_step, 0));
    CPPUNIT_ASSERT_EQUAL(1.0, eval(_step, DBL_EPSILON));

    //trunc moves towards the nearest integer that is smaller in
    //absolute value
    CPPUNIT_ASSERT_EQUAL(0.0, eval(_trunc, 0));
    CPPUNIT_ASSERT_EQUAL(0.0, eval(_trunc, 0.5));
    CPPUNIT_ASSERT_EQUAL(0.0, eval(_trunc, 1 - DBL_EPSILON));
    CPPUNIT_ASSERT_EQUAL(1.0, eval(_trunc, 1));
    CPPUNIT_ASSERT_EQUAL(1.0, eval(_trunc, 1 + DBL_EPSILON));
    CPPUNIT_ASSERT_EQUAL(0.0, eval(_trunc, -0.9));
    CPPUNIT_ASSERT_EQUAL(-1.0, eval(_trunc, -1.9999));
    
    //abs makes negative numbers positive, 
    //leaves non-negative numbers unchanged
    const double x = M_PI;
    CPPUNIT_ASSERT_EQUAL(x, eval(_abs, -x));
    CPPUNIT_ASSERT_EQUAL(x, eval(_abs, x));
    CPPUNIT_ASSERT_EQUAL(0.0, eval(_abs, 0.0));
}

/*

void BUGSFunTest::observable()
{
    const double c1[1] = {1};
    const double c2[2] = {-5.4, 2.3};
    const double badc1[3] = {1.7, 2.7, 2.6};
    const double badc2[2] = {-1, -1};

    vector<double const *> cut1 = mkVec(c1, 1);
    vector<double const *> cut2 = mkVec(c2, 2);
    vector<double const *> cutbad1 = mkVec(badc1, 3);
    vector<double const *> cutbad1 = mkVec(badc2, 2);

    CPPUNIT_ASSERT_EQUAL(0, eval(_interval, -1, 1));
    CPPUNIT_ASSERT_EQUAL(0, eval(_interval, -0.1, 1);
    CPPUNIT_ASSERT_EQUAL(1, eval(_interval, vector<double>(1, 0.1),  cut1));
    CPPUNIT_ASSERT_EQUAL(1, eval(_interval, vector<double>(1, -1), cut1));

    CPPUNIT_ASSERT_EQUAL(0, eval(_interval, vector<double>(1, -1), cut1));
    CPPUNIT_ASSERT_EQUAL(0, eval(_interval, vector<double>(1, -0.1), cut1));
    CPPUNIT_ASSERT_EQUAL(1, eval(_interval, vector<double>(1, 0.1),  cut1));
    CPPUNIT_ASSERT_EQUAL(1, eval(_interval, vector<double>(1, -1), cut1));

    //Observable functions
    _dinterval = new jags::bugs::DIntervalFunc;
    _dround = new jags::bugs::DRoundFunc;
    _dsum = new jags::bugs::DSumFunc;
}
*/

void BugsFunTest::slp()
{
    CPPUNIT_ASSERT(neverslp(_sin, 1));
    CPPUNIT_ASSERT(neverslp(_cos, 1));
    CPPUNIT_ASSERT(neverslp(_tan, 1));
    CPPUNIT_ASSERT(neverslp(_arccos, 1));
    CPPUNIT_ASSERT(neverslp(_arcsin, 1));
    CPPUNIT_ASSERT(neverslp(_arctan, 1));

    CPPUNIT_ASSERT(neverslp(_sinh, 1));
    CPPUNIT_ASSERT(neverslp(_cosh, 1));
    CPPUNIT_ASSERT(neverslp(_tanh, 1));
    CPPUNIT_ASSERT(neverslp(_arccosh, 1));
    CPPUNIT_ASSERT(neverslp(_arcsinh, 1));
    CPPUNIT_ASSERT(neverslp(_arctanh, 1));

    CPPUNIT_ASSERT(neverslp(_cloglog, 1));
    CPPUNIT_ASSERT(neverslp(_log, 1));
    CPPUNIT_ASSERT(neverslp(_logit, 1));
    CPPUNIT_ASSERT(neverslp(_probit, 1));
    CPPUNIT_ASSERT(neverslp(_icloglog, 1));
    CPPUNIT_ASSERT(neverslp(_exp, 1));
    CPPUNIT_ASSERT(neverslp(_ilogit, 1));
    CPPUNIT_ASSERT(neverslp(_phi, 1));

    CPPUNIT_ASSERT(neverslp(_max, 1));
    CPPUNIT_ASSERT(neverslp(_min, 1));
    CPPUNIT_ASSERT(neverslp(_sd, 1));

    CPPUNIT_ASSERT(neverslp(_logfact, 1));
    CPPUNIT_ASSERT(neverslp(_loggam, 1));

    CPPUNIT_ASSERT(neverslp(_round, 1));
    CPPUNIT_ASSERT(neverslp(_step, 1));
    CPPUNIT_ASSERT(neverslp(_trunc, 1));
    CPPUNIT_ASSERT(neverslp(_abs, 1));

    CPPUNIT_ASSERT(neverslp(_dinterval, 2));
    CPPUNIT_ASSERT(neverslp(_dround, 2));

    CPPUNIT_ASSERT(neverslp(_order, 1));
    CPPUNIT_ASSERT(neverslp(_rank, 1));
    CPPUNIT_ASSERT(neverslp(_sort, 1));

    CPPUNIT_ASSERT(neverslp(_inverse, 1));
    CPPUNIT_ASSERT(neverslp(_logdet, 1));

    CPPUNIT_ASSERT(neverslp(_interplin, 3));
}

void BugsFunTest::linear()
{
    CPPUNIT_ASSERT(_mean->isLinear(T, vector<bool>()));
    CPPUNIT_ASSERT(_sum->isLinear(T, vector<bool>()));
    CPPUNIT_ASSERT(neverlinear(_prod, 1));
    CPPUNIT_ASSERT(neverlinear(_sqrt, 1));
    CPPUNIT_ASSERT(_transpose->isLinear(T, vector<bool>()));

    CPPUNIT_ASSERT(_dsum->isLinear(TF, vector<bool>()));
    CPPUNIT_ASSERT(_dsum->isLinear(FT, vector<bool>()));
    CPPUNIT_ASSERT(_dsum->isLinear(TT, vector<bool>()));

    CPPUNIT_ASSERT(_matmult->isLinear(TF, vector<bool>()));
    CPPUNIT_ASSERT(_matmult->isLinear(FT, vector<bool>()));
    CPPUNIT_ASSERT(!_matmult->isLinear(TT, vector<bool>()));

    CPPUNIT_ASSERT(_inprod->isLinear(TF, vector<bool>()));
    CPPUNIT_ASSERT(_inprod->isLinear(FT, vector<bool>()));
    CPPUNIT_ASSERT(!_inprod->isLinear(TT, vector<bool>()));

    CPPUNIT_ASSERT(_ifelse->isLinear(FTT,  vector<bool>()));
    CPPUNIT_ASSERT(_ifelse->isLinear(FTF, vector<bool>()));
    CPPUNIT_ASSERT(_ifelse->isLinear(FFT, vector<bool>()));
    CPPUNIT_ASSERT(!_ifelse->isLinear(TFF,  vector<bool>()));
    CPPUNIT_ASSERT(!_ifelse->isLinear(TFT,  vector<bool>()));
    CPPUNIT_ASSERT(!_ifelse->isLinear(TTF, vector<bool>()));
    CPPUNIT_ASSERT(!_ifelse->isLinear(TTT, vector<bool>()));

    CPPUNIT_ASSERT(!_ifelse->isLinear(FTF, FFT));
    CPPUNIT_ASSERT(!_ifelse->isLinear(FTF, TFF));
    CPPUNIT_ASSERT(!_ifelse->isLinear(FTF, TFT));

    CPPUNIT_ASSERT(!_ifelse->isLinear(FFT, FTF));
    CPPUNIT_ASSERT(!_ifelse->isLinear(FFT, TFF));
    CPPUNIT_ASSERT(!_ifelse->isLinear(FFT, TTF));
}

void BugsFunTest::scale()
{
    CPPUNIT_ASSERT(_mean->isScale(T, vector<bool>()));
    CPPUNIT_ASSERT(_sum->isScale(T, vector<bool>()));
    CPPUNIT_ASSERT(neverscale(_prod, 1));
    CPPUNIT_ASSERT(neverscale(_sqrt, 1));
    CPPUNIT_ASSERT(_transpose->isScale(T, vector<bool>()));

    CPPUNIT_ASSERT(!_dsum->isScale(TF, vector<bool>()));
    CPPUNIT_ASSERT(!_dsum->isScale(FT, vector<bool>()));
    CPPUNIT_ASSERT(_dsum->isScale(TT, vector<bool>()));

    CPPUNIT_ASSERT(_matmult->isScale(TF, vector<bool>()));
    CPPUNIT_ASSERT(_matmult->isScale(FT, vector<bool>()));
    CPPUNIT_ASSERT(!_matmult->isScale(TT, vector<bool>()));

    CPPUNIT_ASSERT(_inprod->isScale(TF, vector<bool>()));
    CPPUNIT_ASSERT(_inprod->isScale(FT, vector<bool>()));
    CPPUNIT_ASSERT(!_inprod->isScale(TT, vector<bool>()));

    CPPUNIT_ASSERT(_ifelse->isScale(FTT,  vector<bool>()));
    CPPUNIT_ASSERT(!_ifelse->isScale(FTF, vector<bool>()));
    CPPUNIT_ASSERT(!_ifelse->isScale(FFT, vector<bool>()));
}

void BugsFunTest::power()
{
    CPPUNIT_ASSERT(neverpow(_mean, 1));
    CPPUNIT_ASSERT(neverpow(_sum, 1));
    CPPUNIT_ASSERT(neverpow(_prod, 1));

    CPPUNIT_ASSERT(_sqrt->isPower(vector<bool>(1,true), vector<bool>()));

    CPPUNIT_ASSERT(neverpow(_transpose, 1));
    CPPUNIT_ASSERT(neverpow(_dsum, 3));
    CPPUNIT_ASSERT(neverpow(_inprod, 2));
}

void BugsFunTest::sort()
{
    double x[8] = {-1.7, 3, 18, 2, -25, 8, 7, 0.5};
    vector<double> arg = mkVec(&x[0], 8);

    vector<double> xord = veval(_order, arg);
    vector<double> xrank = veval(_rank, arg);
    vector<double> xsort = veval(_sort, arg);
    
    for (unsigned int i = 0; i < 8; ++i) {
	unsigned int j = static_cast<unsigned int>(xord[i] - 1);
	unsigned int k = static_cast<unsigned int>(xrank[i] - 1);

	CPPUNIT_ASSERT_EQUAL(static_cast<double>(i), xord[k] - 1);
	CPPUNIT_ASSERT_EQUAL(static_cast<double>(i), xrank[j] - 1);
	CPPUNIT_ASSERT_EQUAL(x[j], xsort[i]);
	CPPUNIT_ASSERT_EQUAL(x[i], xsort[k]);
    }
}

void BugsFunTest::matrix()
{
    //Just basic checks on the identity matrix

    for (unsigned int n = 1; n < 6; ++n) {
	vector<double> A(n*n, 0);
	for (unsigned int i = 0; i < n; ++i) {
	    A[i + n*i] = 1;
	}
	vector<double const *> argA(1, &A[0]);

	vector<unsigned int> dA(n, n);
	vector<vector<unsigned int> > dimA(1, dA);
    
	vector<double> invA(n*n);
	_inverse->checkParameterDim(dimA);
	_inverse->evaluate(&invA[0], argA, dimA);
	for (unsigned int i = 0; i < n; ++i) {
	    CPPUNIT_ASSERT_DOUBLES_EQUAL(A[i], invA[i], tol);
	}
    
	double logdetA;
	_logdet->checkParameterDim(dimA);
	_logdet->evaluate(&logdetA, argA, dimA);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, logdetA, tol);
    
	vector<double const *> argA2(2, &A[0]);
	vector<vector<unsigned int> > dimA2(2, dA);
	vector<double> Asquared(n*n);
	_matmult->checkParameterDim(dimA2);
	_matmult->evaluate(&Asquared[0], argA2, dimA2);
	for (unsigned int i = 0; i < n; ++i) {
	    CPPUNIT_ASSERT_DOUBLES_EQUAL(A[i], Asquared[i], tol);
	}
    
	vector<double> tA(n*n);
	_transpose->checkParameterDim(dimA);
	_transpose->evaluate(&tA[0], argA, dimA);
	for (unsigned int i = 0; i < n; ++i) {
	    CPPUNIT_ASSERT_DOUBLES_EQUAL(A[i], tA[i], tol);
	}
    }
}

void BugsFunTest::inprod()
{
    double x3[4] = {7, 8, 9};
    double y3[4] = {1, 2, -3};

    CPPUNIT_ASSERT(checkparlen(_inprod, 3, 3));
    CPPUNIT_ASSERT(!checkparlen(_inprod, 3, 4));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-4, eval(_inprod, mkVec(x3, 3), mkVec(y3, 3)),
				 tol);

    double x4[4] = {-1, 0, 2.7, 3};
    double y4[4] = {1, -1, 1, -1};
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.3,
				 eval(_inprod, mkVec(x4, 4), mkVec(y4, 4)),
				 tol);
}

void BugsFunTest::interplin()
{
    double c;
    double x[6] = {-10, -0.5, 0, 1.2, 3.8, 77};
    double y[6] = {-2, 3, -6.5, 8, 2.14, 7};

    vector<double const *> arg(3);
    arg[0] = &c;
    arg[1] = x;
    arg[2] = y;

    vector<unsigned int> arglen(3);
    arglen[0] = 1;
    arglen[1] = 6;
    arglen[2] = 6;

    double ans;
    CPPUNIT_ASSERT(_interplin->checkParameterLength(arglen));

    //Exact agreement at break points
    for (unsigned int i = 0; i < 6; ++i) {
	c = x[i];
	_interplin->evaluate(&ans, arg, arglen);
	CPPUNIT_ASSERT_EQUAL(y[i], ans);
    }

    //Interpolation between break points
    unsigned int N = 100;
    for (unsigned int i = 0; i < 5; ++i) {
	for (unsigned int j = 0; j <= N; ++j) {
	    c = ((N - j) * x[i] + j * x[i + 1])/ N;
	    CPPUNIT_ASSERT(_interplin->checkParameterValue(arg, arglen));
	    _interplin->evaluate(&ans, arg, arglen);
	    double w = ((N - j) * y[i] + j * y[i + 1])/ N;
	    CPPUNIT_ASSERT_DOUBLES_EQUAL(w, ans, tol);
	}
    }

    //Extrapolation beyond break points
    c = JAGS_POSINF;
    CPPUNIT_ASSERT(_interplin->checkParameterValue(arg, arglen));
    _interplin->evaluate(&ans, arg, arglen);
    CPPUNIT_ASSERT_EQUAL(y[5], ans);
 
    c = JAGS_NEGINF;
    CPPUNIT_ASSERT(_interplin->checkParameterValue(arg, arglen));
    _interplin->evaluate(&ans, arg, arglen);
    CPPUNIT_ASSERT_EQUAL(y[0], ans);
    
    //CPPUNIT_FAIL("interplin");
}

void BugsFunTest::ifelse()
{
    CPPUNIT_ASSERT_EQUAL(2.0, eval(_ifelse, 0, 1, 2));
    CPPUNIT_ASSERT_EQUAL(1.0, eval(_ifelse, 1, 1, 2));
    CPPUNIT_ASSERT_EQUAL(1.0, eval(_ifelse, -1, 1, 2));
    CPPUNIT_ASSERT_EQUAL(1.0, eval(_ifelse, DBL_EPSILON, 1, 2));
    CPPUNIT_ASSERT_EQUAL(1.0, eval(_ifelse, JAGS_POSINF, 1, 2));
    CPPUNIT_ASSERT_EQUAL(1.0, eval(_ifelse, JAGS_NEGINF, 1, 2));
}

void BugsFunTest::discrete()
{
    //Trigonometric functions and their inverses
    CPPUNIT_ASSERT(isdiscrete(_sin, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_cos, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_tan, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_arccos, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_arcsin, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_arctan, 1, never));

    //Hyperbolic functions and their inverses
    CPPUNIT_ASSERT(isdiscrete(_sinh, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_cosh, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_tanh, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_arcsinh, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_arccosh, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_arctanh, 1, never));

    //Link functions and their inverses
    CPPUNIT_ASSERT(isdiscrete(_cloglog, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_log, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_logit, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_probit, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_icloglog, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_exp, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_ilogit, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_phi, 1, never));

    //Scalar summaries of vectors
    CPPUNIT_ASSERT(isdiscrete(_max, 1, all));
    CPPUNIT_ASSERT(isdiscrete(_mean, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_min, 1, all));
    CPPUNIT_ASSERT(isdiscrete(_sum, 1, all));
    CPPUNIT_ASSERT(isdiscrete(_sd, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_prod, 1, all));

    //Mathematical functions
    CPPUNIT_ASSERT(isdiscrete(_logfact, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_loggam, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_sqrt, 1, never));

    //Lossy scalar functions
    CPPUNIT_ASSERT(isdiscrete(_round, 1, always));
    CPPUNIT_ASSERT(isdiscrete(_step, 1, always));
    CPPUNIT_ASSERT(isdiscrete(_trunc, 1, always));
    CPPUNIT_ASSERT(isdiscrete(_abs, 1, all));

    //Observable functions
    CPPUNIT_ASSERT(isdiscrete(_dinterval, 2, always));
    CPPUNIT_ASSERT(isdiscrete(_dround, 2, never));
    CPPUNIT_ASSERT(isdiscrete(_dsum, 1, all));

//CPPUNIT_ASSERT(isdiscrete(_equals 

    //Sorting functions
    CPPUNIT_ASSERT(isdiscrete(_order, 1, always));
    CPPUNIT_ASSERT(isdiscrete(_rank, 1, always));
    CPPUNIT_ASSERT(isdiscrete(_sort, 1, all));

    //Matrix functions
    CPPUNIT_ASSERT(isdiscrete(_inverse, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_logdet, 1, never));
    CPPUNIT_ASSERT(isdiscrete(_matmult, 2, all));
    CPPUNIT_ASSERT(isdiscrete(_transpose, 1, all));
    CPPUNIT_ASSERT(isdiscrete(_inprod, 2, all));

    //Odds and sods
    CPPUNIT_ASSERT(_ifelse->isDiscreteValued(FTT));
    CPPUNIT_ASSERT(_ifelse->isDiscreteValued(TTT));
    CPPUNIT_ASSERT(!_ifelse->isDiscreteValued(TTF));
    CPPUNIT_ASSERT(!_ifelse->isDiscreteValued(TFT));
    CPPUNIT_ASSERT(!_ifelse->isDiscreteValued(TFF));

    CPPUNIT_ASSERT(isdiscrete(_interplin, 3, never));
}

#include "test.h"

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
#include "DFunction.h"
#include "DIntervalFunc.h"
#include "DPQFunction.h"
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
#include "PFunction.h"
#include "Phi.h"
#include "Probit.h"
#include "Prod.h"
#include "QFunction.h"
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
#include "test.h"
#include "Transpose.h"
#include "Trunc.h"

#include <JRmath.h>

#include <util/nainf.h>
#include <util/integer.h>
using jags::checkInteger;

using std::vector;
using std::string;

#include <climits>
static double tol = 16 * DBL_EPSILON;

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

    //Odds and sods
    delete _ifelse;
    delete _inprod;
    delete _interplin;
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

/*
void BugsFunTest::trig()
{
    double v;
    vector<double const *> args(1);
    args[0] = &v;

    v = 0; 
    CPPUNIT_ASSERT_EQUAL(0.0, _sin->evaluate(args));
    CPPUNIT_ASSERT_EQUAL(1.0, _cos->evaluate(args));
    CPPUNIT_ASSERT_EQUAL(0.0, _tan->evaluate(args));

    v = M_PI / 4;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, _tan->evaluate(args), tol);

    v = M_PI / 2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, _sin->evaluate(args), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, _cos->evaluate(args), tol);

    v = 3 * M_PI / 4;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, _tan->evaluate(args), tol);

    v = M_PI;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, _sin->evaluate(args), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, _cos->evaluate(args), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, _tan->evaluate(args), tol);

    v = 5 * M_PI / 4;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, _tan->evaluate(args), tol);

    v = 3 * M_PI / 2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, _sin->evaluate(args), tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, _cos->evaluate(args), tol);

//    v = 5 * M_PI / 4;
//    CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0, _tan->evaluate(args), tol);

}
*/

BU
    //Link functions and their inverses
    delete _cloglog;
    delete _log;
    delete _logit;
    delete _probit;
    delete _icloglog;
    delete _exp;
    delete _ilogit;
    delete _phi;

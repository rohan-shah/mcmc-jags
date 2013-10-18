#include "test.h"

#include "Add.h"
#include "And.h"
#include "Divide.h"
#include "Equal.h"
#include "GreaterOrEqual.h"
#include "GreaterThan.h"
#include "LessOrEqual.h"
#include "LessThan.h"
#include "Multiply.h"
#include "Neg.h"
#include "NotEqual.h"
#include "Not.h"
#include "Or.h"
#include "Pow.h"
#include "Subtract.h"

#include <util/nainf.h>
#include <util/integer.h>
using jags::checkInteger;

#include <cmath>
using std::pow;
using std::vector;
using std::string;

#include <climits>
static double tol = 16 * DBL_EPSILON;

void BaseFunTest::setUp()
{
    _add = new jags::base::Add;
    _and = new jags::base::And;
    _divide = new jags::base::Divide;
    _equal = new jags::base::Equal;
    _geq = new jags::base::GreaterOrEqual;
    _gt = new jags::base::GreaterThan;
    _leq = new jags::base::LessOrEqual;
    _lt = new jags::base::LessThan;
    _multiply = new jags::base::Multiply;
    _neg = new jags::base::Neg;
    _neq = new jags::base::NotEqual;
    _not = new jags::base::Not;
    _or = new jags::base::Or;
    _pow = new jags::base::Pow;
    _subtract = new jags::base::Subtract;
}


void BaseFunTest::tearDown()
{
    delete _add;
    delete _and;
    delete _divide;
    delete _equal;
    delete _geq;
    delete _gt;
    delete _leq;
    delete _lt;
    delete _multiply;
    delete _neg;
    delete _neq;
    delete _not;
    delete _or;
    delete _pow;
    delete _subtract;
}

void BaseFunTest::testName()
{
    CPPUNIT_ASSERT_EQUAL(string("+"), _add->name());
    CPPUNIT_ASSERT_EQUAL(string("&&"), _and->name());
    CPPUNIT_ASSERT_EQUAL(string("/"), _divide->name());
    CPPUNIT_ASSERT_EQUAL(string("=="), _equal->name());
    CPPUNIT_ASSERT_EQUAL(string(">="), _geq->name());
    CPPUNIT_ASSERT_EQUAL(string(">"), _gt->name());
    CPPUNIT_ASSERT_EQUAL(string("<="), _leq->name());
    CPPUNIT_ASSERT_EQUAL(string("<"), _lt->name());
    CPPUNIT_ASSERT_EQUAL(string("*"), _multiply->name());
    //Unary minus is converted to the token NEG to avoid
    //confusing with binary minus
    CPPUNIT_ASSERT_EQUAL(string("NEG"), _neg->name());
    CPPUNIT_ASSERT_EQUAL(string("!="), _neq->name());
    CPPUNIT_ASSERT_EQUAL(string("!"), _not->name());
    CPPUNIT_ASSERT_EQUAL(string("||"), _or->name());
    CPPUNIT_ASSERT_EQUAL(string("^"), _pow->name());
    CPPUNIT_ASSERT_EQUAL(string("-"), _subtract->name());

    //None of the built-in functions has an alias, except pow
    CPPUNIT_ASSERT_EQUAL(string(""), _add->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _and->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _divide->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _equal->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _geq->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _gt->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _leq->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _lt->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _multiply->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _neg->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _neq->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _not->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _or->alias());
    CPPUNIT_ASSERT_EQUAL(string(""), _subtract->alias());
    CPPUNIT_ASSERT_EQUAL(string("pow"), _pow->alias());
}

void BaseFunTest::arithmetic1(double v)
{
    //Arithmetic functions taking a single argument
    CPPUNIT_ASSERT(_neg->checkNPar(1));
    CPPUNIT_ASSERT(_add->checkNPar(1));
    CPPUNIT_ASSERT(_multiply->checkNPar(1));
    //Binary functions not valid with a single argument
    CPPUNIT_ASSERT(!_subtract->checkNPar(1));
    CPPUNIT_ASSERT(!_divide->checkNPar(1));
    CPPUNIT_ASSERT(!_pow->checkNPar(1));

    vector<double const *> args(1);
    args[0] = &v;

    CPPUNIT_ASSERT_EQUAL(-v, _neg->evaluate(args));
    CPPUNIT_ASSERT_EQUAL(v, _add->evaluate(args));
    CPPUNIT_ASSERT_EQUAL(v, _multiply->evaluate(args));
}

void BaseFunTest::arithmetic2(double v1, double v2)
{
    //Arithmetic functions taking two arguments
    CPPUNIT_ASSERT(_add->checkNPar(2));
    CPPUNIT_ASSERT(_multiply->checkNPar(2));
    CPPUNIT_ASSERT(_subtract->checkNPar(2));
    CPPUNIT_ASSERT(_divide->checkNPar(2));
    CPPUNIT_ASSERT(_pow->checkNPar(2));
    //Unary functions not valid with two arguments
    CPPUNIT_ASSERT(!_neg->checkNPar(2));

    vector<double const *> args(2);
    args[0] = &v1;
    args[1] = &v2;

    CPPUNIT_ASSERT_EQUAL(_add->evaluate(args), v1 + v2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_subtract->evaluate(args),  v1 - v2, tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_multiply->evaluate(args), v1 * v2, tol);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_divide->evaluate(args),   v1 / v2, tol);
    if (v1 < 0 && !checkInteger(v2)) {
	CPPUNIT_ASSERT(jags_isnan(_pow->evaluate(args)));
    }
    else {
	CPPUNIT_ASSERT_DOUBLES_EQUAL(_pow->evaluate(args), pow(v1, v2), tol);
    }
}

void BaseFunTest::arithmetic3(double v1, double v2, double v3)
{
    //Arithmetic functions taking 3 arguments
    CPPUNIT_ASSERT(_add->checkNPar(3));
    CPPUNIT_ASSERT(_multiply->checkNPar(3));
    //These are not valid with three arguments
    CPPUNIT_ASSERT(!_neg->checkNPar(3));
    CPPUNIT_ASSERT(!_subtract->checkNPar(3));
    CPPUNIT_ASSERT(!_divide->checkNPar(3));
    CPPUNIT_ASSERT(!_pow->checkNPar(3));

    vector<double const *> args(3);
    args[0] = &v1;
    args[1] = &v2;
    args[2] = &v3;

    CPPUNIT_ASSERT_EQUAL(_add->evaluate(args), v1 + v2 + v3);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_multiply->evaluate(args), v1 * v2 * v3, tol);
}

void BaseFunTest::testArithmetic()
{
    arithmetic1(0.0);
    arithmetic1(-3.7);
    arithmetic1(28.16);

    arithmetic2( 1.0,  2.0);
    arithmetic2(-1.0,  3.0);
    arithmetic2(-2.5,  8.3);
    arithmetic2(-3.7, -1.9);

    arithmetic3( 0.0, 0.0,    0.0);
    arithmetic3( 1.0, 2.0,    3.0);
    arithmetic3(-1.0, 3.0,  -19.2);
    arithmetic3(-2.5,  8.3, -8.62);
    arithmetic3(-3.7, -1.9, -3.8);
}

void BaseFunTest::logical1()
{
    //Logical Not takes one argument
    CPPUNIT_ASSERT(_not->checkNPar(1));
    //Logical And and Logical Or do not
    CPPUNIT_ASSERT(!_and->checkNPar(1));
    CPPUNIT_ASSERT(!_or->checkNPar(1));
    
    double v;
    vector<double const*> arg(1, &v);

    v = 0; CPPUNIT_ASSERT_EQUAL(1.0, _not->evaluate(arg));
    v = 1; CPPUNIT_ASSERT_EQUAL(0.0, _not->evaluate(arg));
    v = 2; CPPUNIT_ASSERT_EQUAL(0.0, _not->evaluate(arg));
    v = -0.5; CPPUNIT_ASSERT_EQUAL(0.0, _not->evaluate(arg));
}

void BaseFunTest::logical2()
{
    //Logical And and Logical Or take two arguments
    CPPUNIT_ASSERT(_and->checkNPar(2));
    CPPUNIT_ASSERT(_or->checkNPar(2));
    //Logical Not does not
    CPPUNIT_ASSERT(!_not->checkNPar(2));

    double TRUE=1, FALSE=0;

    vector<double const*> FF(2); FF[0] = &FALSE; FF[1] = &FALSE;
    vector<double const*> FT(2); FT[0] = &FALSE; FT[1] = &TRUE;
    vector<double const*> TF(2); TF[0] = &TRUE; TF[1] = &FALSE;
    vector<double const*> TT(2); TT[0] = &TRUE; TT[1] = &TRUE;
    
    CPPUNIT_ASSERT_EQUAL(FALSE, _and->evaluate(FF));
    CPPUNIT_ASSERT_EQUAL(FALSE, _and->evaluate(FT));
    CPPUNIT_ASSERT_EQUAL(FALSE, _and->evaluate(TF));
    CPPUNIT_ASSERT_EQUAL(TRUE,  _and->evaluate(TT));

    CPPUNIT_ASSERT_EQUAL(FALSE, _or->evaluate(FF));
    CPPUNIT_ASSERT_EQUAL(TRUE,  _or->evaluate(FT));
    CPPUNIT_ASSERT_EQUAL(TRUE,  _or->evaluate(TF));
    CPPUNIT_ASSERT_EQUAL(TRUE,  _or->evaluate(TT));

    //Any non-zero value should be interpreted as "true".
    //Try it with a few arbitrary non-zero values.
    double truevals[4] = {-3.872, 1e100, DBL_EPSILON, 3};

    for (unsigned int i = 0; i < 4; ++i) {

	TRUE = truevals[i];
	
	CPPUNIT_ASSERT_EQUAL(0.0, _and->evaluate(FF));
	CPPUNIT_ASSERT_EQUAL(0.0, _and->evaluate(FT));
	CPPUNIT_ASSERT_EQUAL(0.0, _and->evaluate(TF));
	CPPUNIT_ASSERT_EQUAL(1.0, _and->evaluate(TT));
	
	CPPUNIT_ASSERT_EQUAL(0.0, _or->evaluate(FF));
	CPPUNIT_ASSERT_EQUAL(1.0, _or->evaluate(FT));
	CPPUNIT_ASSERT_EQUAL(1.0, _or->evaluate(TF));
	CPPUNIT_ASSERT_EQUAL(1.0, _or->evaluate(TT));
    }
}

void BaseFunTest::logical3()
{
    //None of the logical functions take 3 arguments
    CPPUNIT_ASSERT(!_and->checkNPar(3));
    CPPUNIT_ASSERT(!_or->checkNPar(3));
    CPPUNIT_ASSERT(!_not->checkNPar(3));
}

void BaseFunTest::testLogical()
{
    logical1();
    logical2();
    logical3();
}

void BaseFunTest::comparison1()
{
    //None of the comparison functions take 1 argument
    CPPUNIT_ASSERT(!_equal->checkNPar(1));
    CPPUNIT_ASSERT(!_geq->checkNPar(1));
    CPPUNIT_ASSERT(!_gt->checkNPar(1));
    CPPUNIT_ASSERT(!_leq->checkNPar(1));
    CPPUNIT_ASSERT(!_lt->checkNPar(1));
    CPPUNIT_ASSERT(!_neq->checkNPar(1));
}

void BaseFunTest::comparison2(double v1, double v2)
{
    //All of the comparison functions take 2 arguments
    CPPUNIT_ASSERT(_equal->checkNPar(2));
    CPPUNIT_ASSERT(_geq->checkNPar(2));
    CPPUNIT_ASSERT(_gt->checkNPar(2));
    CPPUNIT_ASSERT(_leq->checkNPar(2));
    CPPUNIT_ASSERT(_lt->checkNPar(2));
    CPPUNIT_ASSERT(_neq->checkNPar(2));

    vector<double const *> args(2);
    args[0] = &v1;
    args[1] = &v2;

    CPPUNIT_ASSERT_EQUAL(static_cast<double>(v1 == v2), _equal->evaluate(args));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(v1 >= v2), _geq->evaluate(args));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(v1 > v2),  _gt->evaluate(args));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(v1 <= v2), _leq->evaluate(args));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(v1 < v2),  _lt->evaluate(args));
    CPPUNIT_ASSERT_EQUAL(static_cast<double>(v1 != v2), _neq->evaluate(args));
}

void BaseFunTest::comparison3()
{
    //None of the comparison functions take 3 arguments
    CPPUNIT_ASSERT(!_equal->checkNPar(3));
    CPPUNIT_ASSERT(!_geq->checkNPar(3));
    CPPUNIT_ASSERT(!_gt->checkNPar(3));
    CPPUNIT_ASSERT(!_leq->checkNPar(3));
    CPPUNIT_ASSERT(!_lt->checkNPar(3));
    CPPUNIT_ASSERT(!_neq->checkNPar(3));
}

void BaseFunTest::testComparison()
{
    comparison1();

    comparison2( 0.0,  0.0);
    comparison2( 1.0,  2.0);
    comparison2( 1.0,  1.0);
    comparison2(-1.0,  3.0);
    comparison2(-2.5,  8.3);
    comparison2(-3.7, -1.9);
    comparison2( -2.8, -2.8);
    comparison2( 0.0, DBL_EPSILON);
    comparison2( DBL_EPSILON, 0.0);
    
    comparison3();
}

void BaseFunTest::discrete1()
{
    vector<bool> T(1, true);
    vector<bool> F(1, false);

    CPPUNIT_ASSERT(!_neg->isDiscreteValued(F));
    CPPUNIT_ASSERT(_neg->isDiscreteValued(T));

    CPPUNIT_ASSERT(_not->isDiscreteValued(F));
    CPPUNIT_ASSERT(_not->isDiscreteValued(T));
}

/*
void BaseFunTest::discrete2()
{

    
    //All of the comparison functions take 2 arguments
    CPPUNIT_ASSERT(_equal->checkNPar(2));
    CPPUNIT_ASSERT(_geq->checkNPar(2));
    CPPUNIT_ASSERT(_gt->checkNPar(2));
    CPPUNIT_ASSERT(_leq->checkNPar(2));
    CPPUNIT_ASSERT(_lt->checkNPar(2));
    CPPUNIT_ASSERT(_neq->checkNPar(2));
}
*/

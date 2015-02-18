#include <function/testfun.h>
#include <util/nainf.h>
#include <util/logical.h>

#include <cppunit/extensions/HelperMacros.h>

using jags::ScalarFunction;
using jags::VectorFunction;
using jags::Function;

#include <climits>
#include <cmath>
#include <algorithm>

using std::vector;
using std::string;
using std::copy;

/* All functions */

bool isdiscrete(Function const *f, bool mask1)
{
    CPPUNIT_ASSERT(checkNPar(f, 1));
    return f->isDiscreteValued(vector<bool>(1, mask1));
}

bool isdiscrete(Function const *f, bool mask1, bool mask2)
{
    CPPUNIT_ASSERT(checkNPar(f, 2));
    vector<bool> arg(2);
    arg[0] = mask1;
    arg[1] = mask2;
    return f->isDiscreteValued(arg);
}

bool isdiscrete(Function const *f, bool mask1, bool mask2, bool mask3)
{
    CPPUNIT_ASSERT(checkNPar(f, 3));
    vector<bool> arg(3);
    arg[0] = mask1;
    arg[1] = mask2;
    arg[2] = mask3;
    return f->isDiscreteValued(arg);
}

class BoolIterator : public std::vector<bool>
{
public:
    bool atEnd;
    BoolIterator(unsigned int n) : vector<bool>(n, false), atEnd(false) {};

    void next() {
	bool bump = true;
	for (unsigned int i = 0; i < size(); ++i) {
	    if (bump) {
		bool x = operator[](i); //current value
		bump = x;
		operator[](i) = !x;
	    }
	    else return;
	}
	if (bump) atEnd=true;
    }
    
};

bool isdiscrete(Function const *f, unsigned int npar,
		bool (*predicate) (vector<bool> const &))
{
    CPPUNIT_ASSERT(checkNPar(f, npar));
    BoolIterator mask(npar);
    
    for(BoolIterator mask(npar); !mask.atEnd; mask.next()) {
	if (f->isDiscreteValued(mask) != predicate(mask)) {
	    return false;
	}
    }
    return true;
}

bool always(vector<bool> const &mask) { return true; }
bool never(vector<bool> const &mask) { return false; }
bool all(vector<bool> const &mask) { return allTrue(mask); }
bool any(vector<bool> const &mask) { return anyTrue(mask); }

bool neverlinear(Function const *f, unsigned int npar)
{
    CPPUNIT_ASSERT_MESSAGE(f->name(), checkNPar(f, npar));

    for(BoolIterator mask(npar); !mask.atEnd; mask.next()) {
	if (f->isLinear(mask, vector<bool>())) return false;
	for(BoolIterator fixed(npar); !fixed.atEnd; fixed.next()) {
	    if (f->isLinear(mask, fixed)) return false;
	}
    }
    return true;
}

bool neverscale(Function const *f, unsigned int npar)
{
    CPPUNIT_ASSERT_MESSAGE(f->name(), checkNPar(f, npar));

    for(BoolIterator mask(npar); !mask.atEnd; mask.next()) {
	if (f->isScale(mask, vector<bool>())) return false;
	for(BoolIterator fixed(npar); !fixed.atEnd; fixed.next()) {
	    if (f->isScale(mask, fixed)) return false;
	}
    }
    return true;
}

bool neverpow(Function const *f, unsigned int npar)
{
    CPPUNIT_ASSERT_MESSAGE(f->name(), checkNPar(f, npar));

    for(BoolIterator mask(npar); !mask.atEnd; mask.next()) {
	if (f->isPower(mask, vector<bool>())) return false;
	for(BoolIterator fixed(npar); !fixed.atEnd; fixed.next()) {
	    if (f->isPower(mask, fixed)) return false;
	}
    }
    return true;
}

bool neverslp(Function const *f, unsigned int npar)
{
    return neverscale(f, npar) && neverlinear(f, npar) && neverpow(f, npar);
}

/* Scalar functions */

void checkLimits(ScalarFunction const *f, double lower, double upper)
{
    CPPUNIT_ASSERT(lower < upper);
    CPPUNIT_ASSERT_MESSAGE(f->name(), checkval(f, lower));
    CPPUNIT_ASSERT_MESSAGE(f->name(), checkval(f, upper));

    CPPUNIT_ASSERT_MESSAGE(f->name(), !jags_isnan(eval(f, lower)));
    CPPUNIT_ASSERT_MESSAGE(f->name(), !jags_isnan(eval(f, upper)));

    if (jags_finite(upper)) {
	if (upper > 1) {
	    upper *= (1.0 + DBL_EPSILON);
	}
	else if (upper < -1) {
	    upper *= (1.0 - DBL_EPSILON);
	}
	else {
	    upper += DBL_EPSILON;
	}
	CPPUNIT_ASSERT_MESSAGE(f->name(), !checkval(f, upper));
    }
    if (jags_finite(lower)) {
	if (lower > 1) {
	    lower *= (1.0 - DBL_EPSILON);
	}
	else if (lower < -1) {
	    lower *= (1.0 + DBL_EPSILON);
	}
	else {
	    lower -= DBL_EPSILON;
	}
	CPPUNIT_ASSERT_MESSAGE(f->name(), !checkval(f, lower));
    }
}

static double checkEval(ScalarFunction const *f,
			vector<double const *> const &args)
{
    //Evaluate scalar function with checks
    CPPUNIT_ASSERT_MESSAGE(f->name(), checkNPar(f, args.size()));
    vector<unsigned int> len(args.size(), 1);
    CPPUNIT_ASSERT_MESSAGE(f->name(), f->checkParameterValue(args));
    return f->evaluate(args);
}

/* 
   Evaluate a scalar function that takes a single argument
*/
double eval(ScalarFunction const *f, const double x)
{
    vector<double const *> arg(1, &x);
    return f->evaluate(arg);
}

/* 
   Evaluate a scalar function that takes two arguments
*/
double eval(ScalarFunction const *f, double x, double y)
{
    CPPUNIT_ASSERT_MESSAGE(f->name(), checkNPar(f, 2));
    vector<double const *> args(2);
    args[0] = &x;
    args[1] = &y;
    CPPUNIT_ASSERT_MESSAGE(f->name(), f->checkParameterValue(args));
    return f->evaluate(args);
}

/* 
   Evaluate a scalar function that takes three arguments
*/
double eval(ScalarFunction const *f, double x, double y, double z)
{
    CPPUNIT_ASSERT_MESSAGE(f->name(), checkNPar(f, 3));
    vector<double const *> args(3);
    args[0] = &x;
    args[1] = &y;
    args[2] = &z;
    CPPUNIT_ASSERT_MESSAGE(f->name(), f->checkParameterValue(args));
    return f->evaluate(args);
}

/*
  Check validity of argument to a scalar function that takes a single
  scalar argument
*/
bool checkval(ScalarFunction const *f, double x)
{
    CPPUNIT_ASSERT_MESSAGE(f->name(), checkNPar(f, 1));
    vector<double const *> arg(1, &x);
    return f->checkParameterValue(arg);
}

bool checkval(ScalarFunction const *f, double x, double y)
{
    CPPUNIT_ASSERT_MESSAGE(f->name(), checkNPar(f, 2));
    vector<double const *> args(2);
    args[0] = &x;
    args[1] = &y;
    return f->checkParameterValue(args);
}

bool checkval(ScalarFunction const *f, double x, double y, double z)
{
    CPPUNIT_ASSERT_MESSAGE(f->name(), checkNPar(f, 3));
    vector<double const *> args(3);
    args[0] = &x;
    args[1] = &y;
    args[2] = &z;
    return f->checkParameterValue(args);
}

/*
static vector<double> mkVec(double x)
{
    return vector<double>(1, x);
}
*/

static vector<double> checkVEval(VectorFunction const *f,
				 vector<double const *> const &args,
				 vector<unsigned int> const &arglen)
{
    // Evaluate vector function with checks 
    CPPUNIT_ASSERT_EQUAL_MESSAGE(f->name(), args.size(), arglen.size());
    CPPUNIT_ASSERT_MESSAGE(f->name(), checkNPar(f, args.size()));
    CPPUNIT_ASSERT_MESSAGE(f->name(), f->checkParameterLength(arglen));
    CPPUNIT_ASSERT_MESSAGE(f->name(), f->checkParameterValue(args, arglen));
    vector<double> ans(f->length(arglen, args));
    f->evaluate(&ans[0], args, arglen);
    return ans;
}

/* Evaluate a VectorFunction that takes a single argument */
vector<double>
veval(VectorFunction const *f, vector<double> const &x)
{
    vector<double const *> arg(1, &x[0]);
    vector<unsigned int> arglen(1, x.size());
    return checkVEval(f, arg, arglen);
}

//Evaluate a VectorFunction that takes two arguments
vector<double>
veval(VectorFunction const *f, vector<double> const &x, vector<double> const &y)
{
    vector<double const *> arg(2);
    arg[0] = &x[0];
    arg[1] = &y[0];
    vector<unsigned int> arglen(2);
    arglen[0] = x.size();
    arglen[1] = y.size();
    return checkVEval(f, arg, arglen);
}

/*
//Evaluate a VectorFunction that takes two arguments, both scalars
vector<double>
veval(VectorFunction const *f, double x, double y)
{
    return veval(f, mkVec(x), mkVec(y));
}

//Evaluate a VectorFunction that takes two arguments, the first is a scalar
vector<double>
veval(VectorFunction const *f, double x, vector<double> const &y)
{
    return veval(f, mkVec(x), y);
}

//Evaluate a VectorFunction that takes two arguments, the second is a scalar
vector<double>
veval(VectorFunction const *f, vector<double> const &x, double y)
{
    return veval(f, x, mkVec(y));
}
*/

/*
  Evaluate a VectorFunction that takes a single argument and returns a
  scalar
*/
double eval(VectorFunction const *f, vector<double> const &x)
{
    vector<double> y = veval(f, x);
    unsigned int N = y.size();
    CPPUNIT_ASSERT_EQUAL(1U, N);
    return y[0];
}

/*
  Evaluate a VectorFunction that takes two arguments and returns a scalar
*/
double eval(VectorFunction const *f, vector<double> const &x, 
	    vector<double> const &y)
{
    vector<double> z = veval(f, x, y);
    unsigned int N = z.size();
    CPPUNIT_ASSERT_EQUAL(1U, N);
    return z[0];
}

/*
  Evaluate a VectorFunction that takes two arguments and returns a
  scalar. The first argument is also a scalar
*/
/*
double eval(VectorFunction const *f, double x, vector<double> const &y)
{
    return eval(f, mkVec(x), y);
}
*/

/*
  Evaluate a VectorFunction that takes two arguments and returns a
  scalar. The second argument is also a scalar
*/
/*
double eval(VectorFunction const *f, vector<double> const &x, double y)
{
    return eval(f, x, mkVec(y));
}
*/

/*
  Check that VectorFunction taking a single argument has valid 
  parameter length
*/
bool checkparlen(VectorFunction const *f, unsigned int n)
{
    vector<unsigned int> arglen(1, n);
    CPPUNIT_ASSERT(checkNPar(f, 1));
    return f->checkParameterLength(arglen);
}

/*
  Check that VectorFunction taking two arguments has valid 
  parameter length
*/
bool checkparlen(VectorFunction const *f, unsigned int n1, unsigned int n2)
{
    vector<unsigned int> arglen(2);
    arglen[0] = n1;
    arglen[1] = n2;
    CPPUNIT_ASSERT(checkNPar(f, 2));
    return f->checkParameterLength(arglen);
}

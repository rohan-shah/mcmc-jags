#ifndef BASE_FUN_TEST_H
#define BASE_FUN_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <testlib.h>

namespace jags {
    class ScalarFunction;
    class VectorFunction;
}

class BaseFunTest : public CppUnit::TestFixture, public JAGSFixture
{
    CPPUNIT_TEST_SUITE( BaseFunTest );
    CPPUNIT_TEST( name );
    CPPUNIT_TEST( alias );
    CPPUNIT_TEST( logical );
    CPPUNIT_TEST( arithmetic );
    CPPUNIT_TEST( comparison );
    CPPUNIT_TEST( discrete );
    CPPUNIT_TEST( slp );
    CPPUNIT_TEST( linear );
    CPPUNIT_TEST( power );
    CPPUNIT_TEST( scale );
    CPPUNIT_TEST( seq );
    CPPUNIT_TEST_SUITE_END();
	    
    jags::ScalarFunction *_add;
    jags::ScalarFunction *_and;
    jags::ScalarFunction *_divide;
    jags::ScalarFunction *_equal;
    jags::ScalarFunction *_geq;
    jags::ScalarFunction *_gt;
    jags::ScalarFunction *_leq;
    jags::ScalarFunction *_lt;
    jags::ScalarFunction *_multiply;
    jags::ScalarFunction *_neg;
    jags::ScalarFunction *_neq;
    jags::ScalarFunction *_not;
    jags::ScalarFunction *_or;
    jags::ScalarFunction *_pow;
    jags::VectorFunction *_seq;
    jags::ScalarFunction *_subtract;

    void arithmetic1(double);
    void arithmetic2(double, double);
    void arithmetic3(double, double, double);

    void logical1();
    void logical2();
    void logical3();

    void comparison1();
    void comparison2(double, double);
    void comparison3();

  public:
    void setUp();
    void tearDown();
    
    void name();
    void alias();
    void logical();
    void arithmetic();
    void comparison();
    void discrete();
    void slp();
    void linear();
    void power();
    void scale();
    void seq();
};

#endif  // BASE_FUN_TEST_H



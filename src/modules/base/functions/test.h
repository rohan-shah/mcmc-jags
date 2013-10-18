#ifndef BASE_FUN_TEST_H
#define BASE_FUN_TEST_H

#include <cppunit/extensions/HelperMacros.h>

namespace jags {
    class ScalarFunction;
}

class BaseFunTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( BaseFunTest );
    CPPUNIT_TEST( testName );
    CPPUNIT_TEST( testLogical );
    CPPUNIT_TEST( testArithmetic );
    CPPUNIT_TEST( testComparison );
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

    void discrete1();

  public:
    void setUp();
    void tearDown();
    
    void testName();
    void testLogical();
    void testArithmetic();
    void testComparison();
};

#endif  // BASE_FUN_TEST_H



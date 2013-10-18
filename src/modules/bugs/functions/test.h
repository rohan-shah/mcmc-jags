#ifndef BUGS_FUN_TEST_H
#define BUGS_FUN_TEST_H

namespace jags {
    class ScalarFunction;
    class VectorFunction;
    class ArrayFunction;
    class LinkFunction;
}

#include <cppunit/extensions/HelperMacros.h>

namespace jags {
    class ScalarFunction;
}

class BugsFunTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( BugsFunTest );
    CPPUNIT_TEST( name );
    CPPUNIT_TEST( alias );
    CPPUNIT_TEST( trig );
    CPPUNIT_TEST_SUITE_END();

    jags::ScalarFunction *_abs;
    jags::ScalarFunction *_arccos;    
    jags::ScalarFunction *_arccosh;
    jags::ScalarFunction *_arcsin;
    jags::ScalarFunction *_arcsinh;
    jags::ScalarFunction *_arctan;
    jags::ScalarFunction *_arctanh;
    jags::ScalarFunction *_cloglog;
    jags::ScalarFunction *_cos;
    jags::ScalarFunction *_cosh;
    jags::VectorFunction *_dinterval;
    jags::ScalarFunction *_dround;
    jags::ArrayFunction  *_dsum;
    jags::LinkFunction   *_exp;
    jags::LinkFunction   *_icloglog;
    jags::ScalarFunction *_ifelse;
    jags::LinkFunction   *_ilogit;
    jags::VectorFunction *_inprod;
    jags::VectorFunction *_interplin;
    jags::ArrayFunction  *_inverse;
    jags::ArrayFunction  *_logdet;
    jags::ScalarFunction *_logfact;
    jags::ScalarFunction *_loggam;
    jags::ScalarFunction *_log;
    jags::ScalarFunction *_logit;
    jags::ArrayFunction  *_matmult;
    jags::VectorFunction *_max;
    jags::VectorFunction *_mean;
    jags::VectorFunction *_min;
    jags::VectorFunction *_order;
    jags::LinkFunction   *_phi;
    jags::ScalarFunction *_probit;
    jags::VectorFunction *_prod;
    jags::VectorFunction *_rank;
    jags::ScalarFunction *_round;
    jags::VectorFunction *_sd;
    jags::ScalarFunction *_sin;
    jags::ScalarFunction *_sinh;
    jags::VectorFunction *_sort;
    jags::ScalarFunction *_sqrt;
    jags::ScalarFunction *_step;
    jags::VectorFunction *_sum;
    jags::ScalarFunction *_tan;
    jags::ScalarFunction *_tanh;
    jags::ArrayFunction  *_transpose;
    jags::ScalarFunction *_trunc;
    jags::ScalarFunction *_yytrunc;
    
  public:
    void setUp();
    void tearDown();
    
    void name();
    void alias();
    void trig();
};

#endif  // BUGS_FUN_TEST_H



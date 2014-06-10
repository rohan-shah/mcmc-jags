#ifndef GLM_SAMP_TEST_H
#define GLM_SAMP_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <testlib.h>

class GLMSampTest : public CppUnit::TestFixture , public JAGSFixture
{
    CPPUNIT_TEST_SUITE( GLMSampTest );
    CPPUNIT_TEST( lgmix );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();
    void lgmix();
};

#endif  // GLM_SAMP_TEST_H



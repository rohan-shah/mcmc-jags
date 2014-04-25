#include "testglm.h"
#include "samplers/testglmsamp.h"
#include <cppunit/extensions/HelperMacros.h>

void init_glm_test() {
    CPPUNIT_TEST_SUITE_REGISTRATION( GLMSampTest );
}

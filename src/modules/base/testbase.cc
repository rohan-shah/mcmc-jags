#include "testbase.h"
#include "functions/testbasefun.h"
#include <cppunit/extensions/HelperMacros.h>

void init_base_test() {
    CPPUNIT_TEST_SUITE_REGISTRATION( BaseFunTest );
}

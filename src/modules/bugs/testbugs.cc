#include "testbugs.h"
#include "functions/testbugsfun.h"
#include <cppunit/extensions/HelperMacros.h>

void init_bugs_test() {
    CPPUNIT_TEST_SUITE_REGISTRATION( BugsFunTest );
}

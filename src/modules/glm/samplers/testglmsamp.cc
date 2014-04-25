#include "testglmsamp.h"
#include "LGMix.h"
#include <JRmath.h>

#include <sstream>
#include <iostream>

using std::vector;
using std::stringstream;

void GLMSampTest::setUp()
{
}

void GLMSampTest::tearDown()
{
}

void GLMSampTest::lgmix()
{
    //CPPUNIT_FAIL("lgmix");
    
    vector<double> means;
    vector<double> weights;
    vector<double> variances;

    for (unsigned int i = 1; i < 30002; ++i) {
	jags::glm::LGMix lg(i);
	lg.getParameters(weights, means, variances);
	double S = 0.0;
	double S2 = 0.0;
	double W = 0.0;
	for (unsigned int i = 0; i < weights.size(); ++i) {
	    W += weights[i];
	    S += weights[i] * means[i];
	}
	for (unsigned int i = 0; i < weights.size(); ++i) {
	    S2 += weights[i] * ((means[i] - S) * (means[i] - S) + variances[i]);
	}

	S += digamma(i);
	S /= sqrt(trigamma(i));
	S2 /= trigamma(i);

	std::stringstream msg;
	msg << "n = ";
	msg << i;

	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str(), 1.0, W, 1.0E-6);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str(), 0.0, S, 1.0E-2);
	CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(msg.str(), 1.0, sqrt(S2), 1.0E-2);
    }
    
}

#include <config.h>
#include "CODA.h"

#include <model/Monitor.h>
#include <util/nainf.h>
#include <util/dim.h>

#include <fstream>
#include <sstream>
#include <vector>

using std::list;
using std::vector;
using std::string;
using std::ostringstream;
using std::ofstream;
using std::ostream;

static void writeDouble(double x, ostream &out)
{
    if (x == JAGS_NA) {
	out << "NA";
    }
    else if (jags_isnan(x)) {
	out << "NaN";
    }
    else if (!jags_finite(x)) {
	if (x > 0) 
	    out << "Inf";
	else 
	    out << "-Inf";
    }
    else {
	out << x;
    }
}

static void WriteIndex(MonitorControl const &control,
		       ofstream &index, unsigned int &lineno)
{
    /* 
       Writes to a coda index file and augments the current line
       number in the corresponding coda output file(s)
    */
    Monitor const *monitor = control.monitor();
    if (monitor->poolIterations()) {
	return;
    }

    unsigned int nvar = product(monitor->dim());
    vector<string> const &enames = monitor->elementNames();
    for (unsigned int i = 0; i < nvar; ++i) {
	index << enames[i] << " " << lineno + 1 << " "
	      << lineno + control.niter() << '\n';
	lineno += control.niter();
    }
}

//Write output file
static void WriteOutput(MonitorControl const &control, int chain,
			ofstream &output)
{
    Monitor const *monitor = control.monitor();
    if (monitor->poolIterations()) {
	return;
    }
    
    vector<double> const &y = monitor->value(chain);
    unsigned int nvar = product(monitor->dim());
    for (unsigned int offset = 0; offset < nvar; ++offset) {
	unsigned int iter = control.start();
	for (unsigned int k = 0; k < control.niter(); ++k) {
	    output << iter << "  ";
	    writeDouble(y[k * nvar + offset], output);
	    output << '\n';
	    iter += control.thin();
	}
    }
}

static void WriteTable(MonitorControl const &control, int chain,
		       ofstream &index)
{
    Monitor const *monitor = control.monitor();
    if (!monitor->poolIterations()) {
	return;
    }

    vector<double> const &y = monitor->value(chain);
    vector<string> const &enames = monitor->elementNames();
    
    unsigned int nvar = product(monitor->dim());
    for (unsigned int i = 0; i < nvar; ++i) {
	index << enames[i] << " " << y[i] << '\n';
    }
}

static bool AnyMonitors(list<MonitorControl> const &mvec,
			bool pooliter, bool poolchains)
{
    /* Check for eligible monitors satisfying poolChains and
     * poolIterations */

    list<MonitorControl>::const_iterator p;
    for (p = mvec.begin(); p != mvec.end(); ++p) {
	if (p->monitor()->poolIterations() == pooliter && 
	    p->monitor()->poolChains() == poolchains) 
	{
	    return true;
	}
    }
    return false;
}

/* CODA output for monitors that do not pool over chains */
void CODA(list<MonitorControl> const &mvec, string const &stem,
	 unsigned int nchain, string &warn)
{
    /* Check for eligible monitors */
    if (!AnyMonitors(mvec, false, false))
	return;

    /* Open index file */
    string iname = stem + "index.txt";
    ofstream index(iname.c_str());
    if (!index) {
	string msg = string("Failed to open file ") + iname + "\n";
	warn.append(msg);
	return;
    }

    /* Open output files */
    vector<ofstream*> output;
    for (unsigned int n = 0; n < nchain; ++n) {
	ostringstream outstream;
	outstream << stem << "chain" << n + 1 << ".txt";
	string oname = outstream.str();
	ofstream *out = new ofstream(oname.c_str());
        if (out) {
            output.push_back(out);
        }
	else {
	    //In case of error, close opened files and return
	    index.close();
	    while(!output.empty()) {
		output.back()->close();
		delete output.back();
		output.pop_back();
	    }
	    string msg = string("Failed to open file ") + oname + "\n";
	    warn.append(msg);
	    return;
	}
    }
    
    unsigned int lineno = 0;
    list<MonitorControl>::const_iterator p;
    for (p = mvec.begin(); p != mvec.end(); ++p) {
	Monitor const *monitor = p->monitor();
	if (!monitor->poolChains() && !monitor->poolIterations()) {
	    WriteIndex(*p, index, lineno);
	    for (unsigned int ch = 0; ch < nchain; ++ch) {
		WriteOutput(*p, ch, *output[ch]);
	    }
	}
    }

    index.close();
    for (unsigned int i = 0; i < nchain; ++i) {
	output[i]->close();
	delete output[i];
    }
}

/* CODA output for monitors that pool over chains */
void CODA0(list<MonitorControl> const &mvec, string const &stem, string &warn)
{
    /* Check for eligible monitors */
    if (!AnyMonitors(mvec, false, true))
	return;

    /* Open index file */
    string iname = stem + "index0.txt";
    ofstream index(iname.c_str());
    if (!index) {
	string msg = string("Failed to open file ") + iname + "\n";
	warn.append(msg);
	return;
    }
    
    /* Open output files */
    string oname = stem + "chain0.txt";
    ofstream output(oname.c_str());
    if (!output) {
	index.close();
	string msg = string("Failed to open file ") + oname + "\n";
	warn.append(msg);
	return;
    }
    
    unsigned int lineno = 0;
    list<MonitorControl>::const_iterator p;
    for (p = mvec.begin(); p != mvec.end(); ++p) {
	Monitor const *monitor = p->monitor();
	if (monitor->poolChains() && !monitor->poolIterations()) {
	    WriteIndex(*p, index, lineno);
	    WriteOutput(*p, 0, output);
	}
    }
    
    index.close();
    output.close();
}

/* TABLE output for monitors that pool over iterations but not over chains
 */
void TABLE(list<MonitorControl> const &mvec, string const &stem,
	  unsigned int nchain, string &warn)
{
    /* Check for eligible monitors */
    if (!AnyMonitors(mvec, true, false))
	return;

    /* Open output files */
    vector<ofstream*> output;
    for (unsigned int n = 0; n < nchain; ++n) {
	ostringstream outstream;
	outstream << stem << "table" << n + 1 << ".txt";
	string oname = outstream.str();
	ofstream *out = new ofstream(oname.c_str());
        if (out) {
            output.push_back(out);
        }
	else {
	    //In case of error, close opened files and return
	    while(!output.empty()) {
		output.back()->close();
		delete output.back();
		output.pop_back();
	    }
	    string msg = string("Failed to open file ") + oname + "\n";
	    warn.append(msg);
	    return;
	}
    }
    
    list<MonitorControl>::const_iterator p;
    for (p = mvec.begin(); p != mvec.end(); ++p) {
	Monitor const *monitor = p->monitor();
	if (!monitor->poolChains() && monitor->poolIterations()) {
	    for (unsigned int ch = 0; ch < nchain; ++ch) {
		WriteTable(*p, ch, *output[ch]);
	    }
	}
    }

    for (unsigned int i = 0; i < nchain; ++i) {
	output[i]->close();
	delete output[i];
    }
}

/* TABLE output for monitors that pool over chains and iterations */
void TABLE0(list<MonitorControl> const &mvec, string const &stem, string &warn)
{
    /* Check for eligible monitors */
    if (!AnyMonitors(mvec, true, true))
	return;

    /* Open output file */
    string iname = stem + "table0.txt";
    ofstream output(iname.c_str());
    if (!output) {
	string msg = string("Failed to open file ") + iname + "\n";
	warn.append(msg);
	return;
    }
    
    list<MonitorControl>::const_iterator p;
    for (p = mvec.begin(); p != mvec.end(); ++p) {
	Monitor const *monitor = p->monitor();
	if (monitor->poolChains() && monitor->poolIterations()) {
	    WriteTable(*p, 0, output);
	}
    }
    
    output.close();
}

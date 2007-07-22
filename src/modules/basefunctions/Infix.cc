#include <config.h>
#include "Infix.h"

using std::vector;
using std::string;

Infix::Infix(string const &name, unsigned int npar)
    : ScalarFunc (name, npar)
{
}

string Infix::deparse(vector<string> const &par) const
{
    string const &fname = this->name();
    string name;
    for (unsigned int i = 0; i < par.size(); ++i) {
	if (i > 0) {
	    name.append(fname);
	}
	name.append(par[i]);
    }
    return name;
}


#include <config.h>
#include <util/dim.h>

#include <set>

using std::vector;
using std::set;

namespace jags {

    vector<unsigned int> const &getUnique(vector<unsigned int> const &dim)
    {
	static set<vector<unsigned int> > _dimset;
	return *(_dimset.insert(dim).first);
    }

    vector<vector<unsigned int> > const &
    getUnique(vector<vector<unsigned int> > const &dimvec)
    {
	static set<vector<vector<unsigned int> > > _dimvecset;
	return *(_dimvecset.insert(dimvec).first);
    }

unsigned int product(vector<unsigned int> const &x)
{
   if (x.empty())
      return 0;

   unsigned int y = x[0];
   for (unsigned int i = 1; i < x.size(); ++i) {
      y *= x[i];
   }
   return y;
}

vector<unsigned int> drop(vector<unsigned int> const &dims)
{
    vector<unsigned int> ans;
    bool empty = true;
    for (unsigned int i = 0; i < dims.size(); ++i) {
	if (dims[i] != 0) {
	    empty = false;
	}
	if (dims[i] != 1) {
	    ans.push_back(dims[i]);
	}
    }
    if (ans.empty() && !empty) {
	ans.push_back(1U);
    }

    return ans;
}

}

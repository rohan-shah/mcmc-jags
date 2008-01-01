#include <config.h>
#include <util/dim.h>

#include <set>

using std::vector;
using std::set;

vector<unsigned int> const &getUnique(vector<unsigned int> const &dim)
{
    static set<vector<unsigned int> > _dimset;
    set<vector<unsigned int> >::const_iterator p = _dimset.find(dim);
    if (p == _dimset.end()) {
	_dimset.insert(dim);
	p = _dimset.find(dim);
    }
    return *p;
}

vector<vector<unsigned int> > const &
getUnique(vector<vector<unsigned int> > const &dimvec)
{
    static set<vector<vector<unsigned int> > > _dimvecset;
    set<vector<vector<unsigned int> > >::const_iterator p = 
       _dimvecset.find(dimvec);
    if (p == _dimvecset.end()) {
	_dimvecset.insert(dimvec);
	p = _dimvecset.find(dimvec);
    }
    return *p;
    
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
    for (unsigned int i = 0; i < dims.size(); ++i) {
	if (dims[i] > 1) 
	    ans.push_back(dims[i]);
    }
    if (ans.empty())
	ans.push_back(1);

    return ans;
}

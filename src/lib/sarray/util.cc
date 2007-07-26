#include <config.h>
#include <sarray/util.h>

using std::vector;

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

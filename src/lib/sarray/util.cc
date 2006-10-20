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

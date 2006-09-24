#ifndef INDEX_H_
#define INDEX_H_

#include <vector> 

class Index : public std::vector<int>
{
   public:
      Index();
      Index(std::vector<int>::size_type size, int t);
};
     
#endif /* INDEX_H_ */

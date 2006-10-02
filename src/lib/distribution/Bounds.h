#ifndef BOUNDS_H_
#define BOUNDS_H_

#include <vector> 

class Distribution;
class SArray;

SArray const *
lowerBound(Distribution const *dist, std::vector<SArray const *> const &args); 

SArray const *
upperBound(Distribution const *dist, std::vector<SArray const *> const &args); 

#endif /* BOUNDS_H_ */

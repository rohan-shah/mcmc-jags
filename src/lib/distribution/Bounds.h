#ifndef BOUNDS_H_
#define BOUNDS_H_

#include <vector> 

class Distribution;

double const *
lowerBound(Distribution const *dist, std::vector<double const *> const &args); 

double const *
upperBound(Distribution const *dist, std::vector<double const *> const &args); 

#endif /* BOUNDS_H_ */

#ifndef DIM_H_
#define DIM_H_

/**
 * @short Some low-level utility functions.
 */
#include <vector>
#include <algorithm>

/**
 * Tests whether the dimension represented by the vector "dim"
 * corresponds to a scalar quantity.
 *
 * Note that isScalar, isVector, and isArray are mutually exclusive.
 */
inline bool isScalar(std::vector<unsigned int> const &dim)
{
   return dim.size() == 1 && dim[0] == 1;
}

/**
 * Tests whether the dimension represented by the vector "dim"
 * corresponds to a vector quantity.

 * Note that isScalar, isVector, and isArray are mutually exclusive.
 */
inline bool isVector(std::vector<unsigned int> const &dim)
{
   return dim.size() == 1 && dim[0] > 1;
}

/**
 * Tests whether the dimension represented by the vector "dim"
 * corresponds to an array.
 *
 * Note that isScalar, isVector, and isArray are mutually exclusive.
 */
inline bool isArray(std::vector<unsigned int> const &dim)
{
   return dim.size() > 1;
}

/**
 * Tests whether the dimension represented by the vector "dim"
 * corresponds to a two-dimensional array.
 */
inline bool isMatrix(std::vector<unsigned int> const &dim)
{
   return dim.size() == 2;
}

/**
 * Tests whether the dimension represented by the vector "dim"
 * corresponds to a matrix with the same number of rows as columns.
 */
inline bool isSquareMatrix(std::vector<unsigned int> const &dim)
{
   return dim.size() == 2 && dim[0] == dim[1];
}

/**
 * Returns the product of the elements of a vector. The most common
 * usage of this function in the JAGS library is to calculate the
 * number of elements in an array, given its dimensions.
 */
unsigned int product(std::vector<unsigned int> const &arg);

/**
 * Drops redundant dimensions
 */
std::vector<unsigned int> drop(std::vector<unsigned int> const &dims);

/**
 * Returns a constant reference to a unique vector
 *
 * Vectors of unsigned integers are frequently repeated objects in
 * the JAGS library (Typically as dimensions of Nodes). This function
 * creates a unique constant reference to the requested vector. This
 * avoids redundant copies of the vector taking up memory.
 */
std::vector<unsigned int> const &getUnique(std::vector<unsigned int> const &dim);

/**
 * Returns a constant reference to a unique vector
 *
 * Vectors of parameter dimensions are frequently repeated objects
 * in the JAGS library. This function returns a reference to a unique
 * copy of the requested vector in order to save memory.
 * 
 */
std::vector<std::vector<unsigned int> > const & 
getUnique(std::vector<std::vector<unsigned int> > const &dimvec);

#endif /* DIM_H_ */

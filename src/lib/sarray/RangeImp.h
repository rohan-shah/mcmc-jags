#ifndef RANGE_IMP_H_
#define RANGE_IMP_H_

#include <vector>

/**
 * A RangeImp implements the interface described by the Range class.
 * All of the member functions of Range are constant, so identical
 * Range objects can share memory.
 *
 * RangeImp objects have a reference count, recording the number of
 * Range objects that use this particular implementation (However they 
 * do no memory management).
 * 
 * @see Range
 */
class RangeImp {
    std::vector<int> _lower, _upper;
    std::vector<unsigned int> _dim, _dim_dropped;
    unsigned int _length, _ref;
    /** 
     * Forbid copying. We only need one instance of a RangeImp object
     * with the same upper and lower limits 
     */
    RangeImp(RangeImp const &rhs);
    RangeImp &operator=(RangeImp const &rhs);
public:
    /** @see Range##Range */
    RangeImp(std::vector<int> const &lower, std::vector<int> const &upper);
    /** Increment reference count, returning new value */
    unsigned int ref();
    /** Decrement reference count, returning new value */
    unsigned int unRef();
    /** @see Range##length */
    unsigned int length() const;
    /** @see Range##contains */
    bool contains(RangeImp const &range) const;
    /** @see Range##overlaps */
    bool overlaps(RangeImp const &range) const;
    /** @see Range##leftOffset */
    unsigned int leftOffset(std::vector<int> const &index) const;
    /** @see Range##leftIndex */
    std::vector<int> leftIndex(unsigned int offset) const;
    /** @see Range##rightOffset */
    unsigned int rightOffset(std::vector<int> const &index) const;
    /** @see Range##rightIndex */
    std::vector<int> rightIndex(unsigned int offset) const;
    /** @see Range##dim */
    std::vector<unsigned int> const & dim(bool drop) const;
    /** @see Range##ndim */
    unsigned int ndim(bool drop) const;
    /** @see Range##lower */
    std::vector<int> const & lower() const;
    /** @see Range##upper */
    std::vector<int> const & upper() const;
    /** @see Range##operator< */
    bool operator<(RangeImp const &rhs) const;
};


#endif /* RANGE_IMP_H_ */

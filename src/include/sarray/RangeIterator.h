#ifndef RANGE_ITERATOR_H_
#define RANGE_ITERATOR_H_

#include <vector>

#include <sarray/Range.h>

/**
 * A RangeIterator is an index that is bound to be inside a given
 * Range.  It has operators to allow traversing the Range in row- or
 * column-major order.
 *
 * @see Range
 */
class RangeIterator : public std::vector<int> {
    const Range _range;
    bool _atend;
    //Forbid assignment
    RangeIterator &operator=(std::vector<int> const &);
    RangeIterator &operator=(RangeIterator const &);
public:
    /**
     * Constructor. The initial value of a RangeIterator is
     * the lower limit of the range argument.
     *
     * @param range. Range to traverse
     */
    RangeIterator(Range const &range);
    virtual ~RangeIterator();
    /**
     * Tests to see if the RangeIterator has gone past the upper limit
     * of the range. For use in for() loops.
     */
    bool atEnd() const;
    /**
     * Goes to the next index in column-major order, (i.e. moving the
     * left hand index fastest). If the RangeIterator at the upper
     * limit of the Range, then a call to nextLeft will move it to the
     * lower limit.
     * 
     * @return reference to self after incrementation
     * @see nextRight
     */
    RangeIterator &nextLeft();
    /**
     * Goes to the next index in row-major order (i.e. moving the right
     * hand index fastest) but otherwise behaves line nextLeft.
     *
     * @return reference to self after incrementation
     * @see nextLeft
     */
    RangeIterator &nextRight();
};

#endif /* RANGE_ITERATOR_H_ */

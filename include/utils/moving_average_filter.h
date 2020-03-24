/*
 * movingAverageFilter.h
 *
 * Moving average filters designed to process either float or integers.
 * Mainly used for smoothing readings of Potentionmeters and external CVs.
 */

#ifndef MOVINGAVERAGEFILTER_H_
#define MOVINGAVERAGEFILTER_H_

#include <cstddef>
#include <cstdint>

namespace fieldkitfx {

/*
 * 1. Moving average filter stores sum of all values, so a larger data
 * type size may be required.
 * 2. It's better to use powers of 2 as length with integer data types,
 * as this allows compilers to replace division with bitwise shift
 * 3. Floating point data types are expected to have some drift
 * in their output due to accumulated rounding errors
 */

template <class T, size_t length>
class MovingAverageFilter {
private:
    // Template members don't get initialized to 0 by default, possibly GCC bug
    T line[length] {};
    T sum {};
    size_t position {};

public:
    T process(T input_sample) {
        // update sum sum
        // sum = sum - line[length - 1] + input_sample;
        sum = sum + input_sample - line[position];
        line[position++] = input_sample;
        if (position == length) {
            position = 0;
        }

        // shift the line
        /*
        T* tmp = &(line[length - 1]);
        while (tmp != line) {
            *tmp = *(--tmp);
        }
        */

        // insert the new sample
        //*tmp = input_sample;
        // return average
        return sum / length;
    }
};
} // namespace fieldkitfx
#endif /* MOVINGAVERAGEFILTER_H_ */

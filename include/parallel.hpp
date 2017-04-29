#pragma once


#include <utility>
#include <iterator>
#include <algorithm>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"


namespace helene
{


template <class InputIterator1, class InputIterator2>
InputIterator1
remove_by_index(InputIterator1 begin,
                InputIterator1 end,
                InputIterator2 index_begin)
{
}


// remove elements in same positions in both ranges according to value in first
// range.
template <class InputIterator1, class InputIterator2, class T>
std::pair<InputIterator1, InputIterator2>
parallel_remove(InputIterator1 begin1,
                InputIterator1 end1,
                InputIterator2 begin2,
                const T& value)
{
    auto found1 = begin1;

    auto temp_end1 = end1;
    auto temp_end2 = begin2;
    std::advance(temp_end2, std::distance(begin1, end1));

    while(found1 != end1)
    {
        found1 = std::find(begin1, end1, value);

        temp_end1 = std::remove_if(
            begin1,
            temp_end1,
            [found1](
                typename std::iterator_traits<InputIterator1>::reference elm) {
                if(&elm == std::addressof(*found1))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            });

        auto found2 = begin2;
        std::advance(found2, std::distance(begin1, found1));

        temp_end2 = std::remove_if(
            begin2,
            temp_end2,
            [found2](
                typename std::iterator_traits<InputIterator2>::reference elm) {

                if(&elm == std::addressof(*found2))
                {
                    return true;
                }
                else
                {
                    return false;
                }

            });
    }

    return std::make_pair(temp_end1, temp_end2);
}


// remove elements in both ranges according to predicate on first range.
template <class InputIterator1, class InputIterator2, class UnaryPredicate>
std::pair<InputIterator1, InputIterator2>
parallel_remove_if(InputIterator1 begin1,
                   InputIterator1 end1,
                   InputIterator2 begin2,
                   UnaryPredicate p)
{
}
}


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
    // initialize new end with initial end
    auto new_end1 = end1;

    // calculate end of range 2 and set new_end2 with it
    auto new_end2 = begin2;
    std::advance(new_end2, std::distance(begin1, end1));

    // find first occurrence of value
    auto found1 = std::find(begin1, new_end1, value);

    // if value was found, continue
    while(found1 != new_end1)
    {
        // make sure to remove only from range until new end and not original
        // end
        new_end1 = std::remove_if(begin1,
                                  new_end1,
                                  [found1](const typename std::iterator_traits<
                                           InputIterator1>::reference elm) {
                                      // remove based on address of found
                                      // element
                                      if(&elm == std::addressof(*found1))
                                      {
                                          return true;
                                      }
                                      else
                                      {
                                          return false;
                                      }
                                  });

        // construct corresponding iterator in range 2
        auto corresponding = begin2;
        std::advance(corresponding, std::distance(begin1, found1));

        new_end2 = std::remove_if(
            begin2,
            new_end2,
            [corresponding](
                const typename std::iterator_traits<InputIterator2>::reference
                    elm) {
                // remove based on address of corresponding element
                if(&elm == std::addressof(*corresponding))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            });

        // continue attempting to find value in range 1
        found1 = std::find(begin1, new_end1, value);
    }

    return std::make_pair(new_end1, new_end2);
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


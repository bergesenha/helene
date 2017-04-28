#pragma once


#include <utility>
#include <iterator>
#include <algorithm>
#include <vector>


namespace helene
{


template <class InputIterator1, class InputIterator2>
InputIterator1
remove_by_index(InputIterator1 begin,
                InputIterator1 end,
                InputIterator2 index_begin)
{
}


template <class InputIterator1, class InputIterator2, class T>
std::pair<InputIterator1, InputIterator2>
parallel_remove(InputIterator1 begin1,
                InputIterator1 end1,
                InputIterator2 begin2,
                const T& value)
{
}


template <class InputIterator1, class InputIterator2, class UnaryPredicate>
std::pair<InputIterator1, InputIterator2>
parallel_remove_if(InputIterator1 begin1,
                   InputIterator1 end1,
                   InputIterator2 begin2,
                   UnaryPredicate p)
{
}
}

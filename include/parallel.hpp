#pragma once


#include <utility>

namespace helene
{


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

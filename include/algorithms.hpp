#pragma once


namespace helene
{
template <class Iterator1, class Iterator2, class BinaryPredicate>
void
parallel_for_each(Iterator1 first1,
                  Iterator1 last1,
                  Iterator2 first2,
                  BinaryPredicate func)
{
    for(; first1 != last1; ++first1, ++first2)
    {
        func(*first1, *first2);
    }
}
}

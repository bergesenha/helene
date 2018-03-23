#pragma once

#include <type_traits>
#include <iterator>


namespace helene
{
namespace detail
{

template <class Iterator, class IteratorTag>
constexpr bool is_iterator_of_category_v =
    std::is_same<typename std::iterator_traits<Iterator>::iterator_category,
                 IteratorTag>::value;

} // namespace detail
/** \class circular_iterator circular_iterator.hpp <circular_iterator.hpp>
 *
 * \brief An iterator adaptor that adapts any iterator to wrap around when
 * incremented beyond a range determined on construction
 */
template <class BaseIterator>
class circular_iterator
{
public:
private:
};
} // namespace helene

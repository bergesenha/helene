#pragma once

#include <type_traits>
#include <iterator>
#include <utility>


namespace helene
{
namespace detail
{

template <class Iterator, class IteratorTag>
constexpr bool is_iterator_of_category_v =
    std::is_same<typename std::iterator_traits<Iterator>::iterator_category,
                 IteratorTag>::value;

template <class Iterator, class IteratorTag>
constexpr bool is_at_least_iterator_of_category_v = std::is_base_of<
    IteratorTag,
    typename std::iterator_traits<Iterator>::iterator_category>::value;

// undefined base, specialized for each iterator category
template <class UnderlyingIterator, class = void>
class circular_iterator_base;

} // namespace detail
/** \class circular_iterator circular_iterator.hpp <circular_iterator.hpp>
 *
 * \brief An iterator adaptor that adapts any iterator to wrap around when
 * incremented beyond a range determined on construction
 */
template <class UnderlyingIterator>
class circular_iterator
    : public detail::circular_iterator_base<UnderlyingIterator>
{
public:
    static_assert(
        detail::is_at_least_iterator_of_category_v<UnderlyingIterator,
                                                   std::forward_iterator_tag>,
        "UnderlyingIterator needs to at least satisfy ForwardIterator for its "
        "multipass ability");

private:
};
} // namespace helene

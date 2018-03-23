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


// specialization for UnderlyingIterator satisfying ForwardIterator
template <class UnderlyingIterator>
class circular_iterator_base<
    UnderlyingIterator,
    std::enable_if_t<is_iterator_of_category_v<UnderlyingIterator,
                                               std::forward_iterator_tag>>>
{
public:
    // inherited access to types from traits
    typedef typename std::iterator_traits<UnderlyingIterator>::value_type
        value_type;
    typedef typename std::iterator_traits<UnderlyingIterator>::difference_type
        difference_type;
    typedef
        typename std::iterator_traits<UnderlyingIterator>::reference reference;
    typedef typename std::iterator_traits<UnderlyingIterator>::pointer pointer;
    typedef typename std::iterator_traits<UnderlyingIterator>::iterator_category
        iterator_category;

public:
    circular_iterator_base() = default;

    circular_iterator_base(UnderlyingIterator current) : current_(current)
    {
    }

public:
    // ForwardIterator conformance
    bool
    operator==(const circular_iterator_base& other) const
    {
        return current_ == other.current_;
    }

    bool
    operator!=(const circular_iterator_base& other) const
    {
        return current_ != other.current_;
    }

    reference operator*()
    {
        return *current_;
    }

    pointer operator->()
    {
        return current_.operator->();
    }


protected:
    UnderlyingIterator current_;
};
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

public:
    circular_iterator() = default;

    circular_iterator(UnderlyingIterator first,
                      UnderlyingIterator last,
                      UnderlyingIterator current)
        : detail::circular_iterator_base<UnderlyingIterator>(current),
          first_(first),
          last_(last)
    {
    }

private:
    UnderlyingIterator first_;
    UnderlyingIterator last_;
};
} // namespace helene

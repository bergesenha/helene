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
} // namespace detail


/** \class circular_iterator circular_iterator.hpp <circular_iterator.hpp>
 *
 * \brief An iterator adaptor that adapts any iterator to wrap around when
 * incremented beyond a range determined on construction
 */
template <class UnderlyingIterator>
class circular_iterator
{
public:
    static_assert(
        detail::is_at_least_iterator_of_category_v<UnderlyingIterator,
                                                   std::forward_iterator_tag>,
        "UnderlyingIterator needs to at least satisfy ForwardIterator for its "
        "multipass ability");

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
    circular_iterator() = default;

    circular_iterator(UnderlyingIterator first,
                      UnderlyingIterator last,
                      UnderlyingIterator current)
        : first_(first), last_(last), current_(current)
    {
    }

    bool
    operator==(const circular_iterator& other)
    {
        return current_ == other.current_;
    }

    bool
    operator!=(const circular_iterator& other)
    {
        return current_ != other.current_;
    }

    reference operator*()
    {
        return *current_;
    }

    circular_iterator&
    operator++()
    {
        ++current_;

        if(current_ == last_)
        {
            current_ = first_;
        }

        return *this;
    }

    circular_iterator
    operator++(int)
    {
        auto temp = *this;
        ++(*this);
        return temp;
    }

private:
    UnderlyingIterator first_;
    UnderlyingIterator last_;
    UnderlyingIterator current_;
};
} // namespace helene

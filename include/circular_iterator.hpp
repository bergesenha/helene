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
template <class UnderlyingIterator,
          class Category = typename std::iterator_traits<
              UnderlyingIterator>::iterator_category>
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

protected:
    UnderlyingIterator first_;
    UnderlyingIterator last_;
    UnderlyingIterator current_;
};


template <class UnderlyingIterator>
class circular_iterator<UnderlyingIterator, std::bidirectional_iterator_tag>
    : public circular_iterator<UnderlyingIterator, std::forward_iterator_tag>
{
public:
    using circular_iterator<UnderlyingIterator,
                            std::forward_iterator_tag>::circular_iterator;

    circular_iterator&
    operator--()
    {
        if(circular_iterator::current_ == circular_iterator::first_)
        {
            // needs to point to last element, not one past last
            circular_iterator::current_ = circular_iterator::last_;
        }

        --circular_iterator::current_;

        return *this;
    }

    circular_iterator
    operator--(int)
    {
        auto temp = *this;
        --(*this);
        return temp;
    }
};


template <class UnderlyingIterator>
class circular_iterator<UnderlyingIterator, std::random_access_iterator_tag>
    : public circular_iterator<UnderlyingIterator,
                               std::bidirectional_iterator_tag>
{
public:
    using circular_iterator<UnderlyingIterator,
                            std::bidirectional_iterator_tag>::circular_iterator;

    circular_iterator&
    operator+=(typename circular_iterator::difference_type n)
    {
        // find remainder in case of several round trips
        const auto length =
            circular_iterator::last_ - circular_iterator::first_;

        const auto current_index =
            circular_iterator::current_ - circular_iterator::first_;

        const auto new_index = (current_index + n) % length;

        if(new_index < 0)
        {
            circular_iterator::current_ = circular_iterator::last_ + new_index;
        }
        else
        {
            circular_iterator::current_ = circular_iterator::first_ + new_index;
        }

        return *this;
    }

    circular_iterator&
    operator-=(typename circular_iterator::difference_type n)
    {
        return operator+=(-n);
    }

    friend circular_iterator
    operator+(const circular_iterator& lhs,
              typename circular_iterator::difference_type rhs)
    {
        auto temp = lhs;
        temp += rhs;
        return temp;
    }

    friend circular_iterator
    operator+(typename circular_iterator::difference_type lhs,
              const circular_iterator& rhs)
    {
        return rhs + lhs;
    }
};
} // namespace helene

#pragma once

#include <iterator>


namespace helene
{

template <class IteratorCategory, class IteratorToStruct, class MemberType>
class member_iterator_base;


template <class IteratorToStruct, class MemberType>
class member_iterator_base<std::input_iterator_tag,
                           IteratorToStruct,
                           MemberType>
{
public:
    member_iterator_base(IteratorToStruct it) : current_(it)
    {
    }

    bool
    operator!=(const member_iterator_base& other) const
    {
        return current_ != other.current_;
    }

    void
    increment()
    {
        ++current_;
    }

    IteratorToStruct
    post_increment()
    {
        return current_++;
    }

protected:
    IteratorToStruct current_;
};


template <class IteratorToStruct, class MemberType>
class member_iterator_base<std::forward_iterator_tag,
                           IteratorToStruct,
                           MemberType>
    : public member_iterator_base<std::input_iterator_tag,
                                  IteratorToStruct,
                                  MemberType>
{
public:
    using member_iterator_base<std::input_iterator_tag,
                               IteratorToStruct,
                               MemberType>::member_iterator_base;
};


template <class IteratorToStruct, class MemberType>
class member_iterator_base<std::bidirectional_iterator_tag,
                           IteratorToStruct,
                           MemberType>
    : public member_iterator_base<std::forward_iterator_tag,
                                  IteratorToStruct,
                                  MemberType>
{
public:
    using member_iterator_base<std::forward_iterator_tag,
                               IteratorToStruct,
                               MemberType>::member_iterator_base;

    void
    decrement()
    {
        --this->current_;
    }

    IteratorToStruct
    post_decrement()
    {
        return this->current_--;
    }
};


template <class IteratorToStruct, class MemberType>
class member_iterator_base<std::random_access_iterator_tag,
                           IteratorToStruct,
                           MemberType>
    : public member_iterator_base<std::bidirectional_iterator_tag,
                                  IteratorToStruct,
                                  MemberType>
{
public:
    typedef typename std::iterator_traits<IteratorToStruct>::difference_type
        difference_type;

    typedef MemberType& reference;

    typedef
        typename std::iterator_traits<IteratorToStruct>::value_type struct_type;

public:
    using member_iterator_base<std::bidirectional_iterator_tag,
                               IteratorToStruct,
                               MemberType>::member_iterator_base;

public:
    void
    operator_pluss_equals(difference_type n)
    {
        this->current_ += n;
    }

    IteratorToStruct
    operator_pluss_number(difference_type n) const
    {
        return this->current_ + n;
    }

    struct_type&
    array_access(difference_type n)
    {
        return this->current_[n];
    }

    bool
    operator<(const member_iterator_base& other) const
    {
        return this->current_ < other.current_;
    }

    bool
    operator>(const member_iterator_base& other) const
    {
        return this->current_ > other.current_;
    }

    bool
    operator<=(const member_iterator_base& other) const
    {
        return this->current_ <= other.current_;
    }

    bool
    operator>=(const member_iterator_base& other) const
    {
        return this->current_ >= other.current_;
    }
};

template <class MemberType,
          class StructType,
          class IteratorToStruct,
          MemberType StructType::*PtrValue>
class member_iterator
    : public member_iterator_base<
          typename std::iterator_traits<IteratorToStruct>::iterator_category,
          IteratorToStruct,
          MemberType>
{
public:
    typedef MemberType value_type;
    typedef MemberType& reference;
    typedef MemberType* pointer;

    typedef typename std::iterator_traits<IteratorToStruct>::difference_type
        difference_type;

    typedef typename std::iterator_traits<IteratorToStruct>::iterator_category
        iterator_category;

public:
    // using base class constructor directly
    using member_iterator_base<
        typename std::iterator_traits<IteratorToStruct>::iterator_category,
        IteratorToStruct,
        MemberType>::member_iterator_base;

public:
    reference operator*()
    {
        return *(this->current_).*PtrValue;
    }

    pointer operator->()
    {
        return &(*(this->current_).*PtrValue);
    }

    member_iterator& operator++()
    {
        this->increment();
        return *this;
    }

    member_iterator operator++(int)
    {
        IteratorToStruct temp = this->post_increment();
        return member_iterator(temp);
    }

    member_iterator& operator--()
    {
        this->decrement();
        return *this;
    }

    member_iterator operator--(int)
    {
        IteratorToStruct temp = this->post_decrement();
        return member_iterator(temp);
    }

    member_iterator&
    operator+=(difference_type n)
    {
        this->operator_pluss_equals(n);
        return *this;
    }

    member_iterator
    operator+(difference_type n) const
    {
        return member_iterator(this->operator_pluss_number(n));
    }

    friend member_iterator
    operator+(difference_type lhs, const member_iterator& rhs)
    {
        return rhs + lhs;
    }

    member_iterator&
    operator-=(difference_type n)
    {
        (*this) += -n;
        return *this;
    }

    member_iterator
    operator-(difference_type n) const
    {
        return *this + (-n);
    }

    reference operator[](difference_type n)
    {
        return this->array_access(n).*PtrValue;
    }
};
}


namespace std
{

template <class MemberType,
          class StructType,
          class IteratorToStruct,
          MemberType StructType::*PtrValue>
struct iterator_traits<
    helene::member_iterator<MemberType, StructType, IteratorToStruct, PtrValue>>
{
    typedef typename std::iterator_traits<IteratorToStruct>::difference_type
        difference_type;
    typedef MemberType value_type;
    typedef MemberType* pointer;
    typedef MemberType& reference;
    typedef typename std::iterator_traits<IteratorToStruct>::iterator_category
        iterator_category;
};
}

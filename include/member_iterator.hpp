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
    typedef MemberType value_type;
    typedef MemberType& reference;
    typedef MemberType* pointer;

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
};
}

#pragma once

#include <iterator>


namespace helene
{

template <class IteratorCategory, class IteratorToStruct>
class member_iterator_base;


template <class IteratorToStruct>
class member_iterator_base<std::input_iterator_tag, IteratorToStruct>
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

protected:
    IteratorToStruct current_;
};


template <class IteratorToStruct>
class member_iterator_base<std::forward_iterator_tag, IteratorToStruct>
    : public member_iterator_base<std::input_iterator_tag, IteratorToStruct>
{
public:
    using member_iterator_base<std::input_iterator_tag,
                               IteratorToStruct>::member_iterator_base;
};


/*
template <class IteratorToStruct>
class member_iterator_base<std::random_access_iterator_tag, IteratorToStruct>
{
public:
    member_iterator_base(IteratorToStruct it) : current_(it)
    {
    }

protected:
    IteratorToStruct current_;
};
*/


template <class MemberType,
          class StructType,
          class IteratorToStruct,
          MemberType StructType::*PtrValue>
class member_iterator
    : public member_iterator_base<
          typename std::iterator_traits<IteratorToStruct>::iterator_category,
          IteratorToStruct>
{
public:
    // using base class constructor directly
    using member_iterator_base<
        typename std::iterator_traits<IteratorToStruct>::iterator_category,
        IteratorToStruct>::member_iterator_base;
};
}

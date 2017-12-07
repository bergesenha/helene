#pragma once

#include <vector>


namespace helene
{

namespace soa_detail
{
template <class MemberPointerType>
struct deduce_member_pointer;

template <class MemberType, class ObjectType>
struct deduce_member_pointer<MemberType ObjectType::*>
{
    typedef MemberType member_type;
    typedef ObjectType object_type;
};

template <class MemberPointerType>
using member_type_t =
    typename deduce_member_pointer<MemberPointerType>::member_type;

template <class MemberPointerType>
using object_type_t =
    typename deduce_member_pointer<MemberPointerType>::object_type;


template <class MemberPointerType, MemberPointerType MemberPointerValue>
class member_container
{
public:
    typedef member_type_t<MemberPointerType> member_type;
    typedef object_type_t<MemberPointerType> object_type;

private:
};
}


class soa
{
public:
private:
};
}

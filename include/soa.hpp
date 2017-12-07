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
}


template <class MemberPointerType, MemberPointerType MemberPointerValue>
class member_container
{
public:
    typedef soa_detail::member_type_t<MemberPointerType> member_type;
    typedef soa_detail::object_type_t<MemberPointerType> object_type;

public:
    member_container() = default;

private:
    std::vector<member_type> members_;
};


// helper to access respective parents that are multiply inherited
template <class FirstParent, class... RestParenst>
struct parent_helper
{
};

template <class LastParent>
struct parent_helper<LastParent>
{
};

template <class T, class... MemberContainer>
class soa : public MemberContainer...
{
public:
    soa() : MemberContainer::MemberContainer()...
    {
    }

private:
};
}

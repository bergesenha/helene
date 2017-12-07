#pragma once

#include <vector>
#include <tuple>
#include <utility>


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


template <class PointerType, PointerType PointerValue>
struct member_reference
{
    static constexpr PointerType pointer_value = PointerValue;
};


template <class T, class... MemberRefs>
class element_proxy;

template <class T, class... PointerTypes, PointerTypes... PointerValues>
class element_proxy<T, member_reference<PointerTypes, PointerValues>...>
{
    template <std::size_t... S>
    T
    make_dispatch(std::index_sequence<S...>)
    {
        return T{*std::get<S>(pointers_)...};
    }

    template <class FirstRef, class... RestRefs>
    struct assignment_helper
    {
        template <std::size_t S, std::size_t... Ss>
        static void
        assign(std::tuple<soa_detail::member_type_t<PointerTypes>*...>& tpl,
               const T& value,
               std::index_sequence<S, Ss...>)
        {
            *std::get<S>(tpl) = value.*FirstRef::pointer_value;

            assignment_helper<RestRefs...>::assign(
                tpl, value, std::index_sequence<Ss...>());
        }
    };

    template <class LastRef>
    struct assignment_helper<LastRef>
    {

        template <std::size_t S>
        static void
        assign(std::tuple<soa_detail::member_type_t<PointerTypes>*...>& tpl,
               const T& value,
               std::index_sequence<S>)
        {

            *std::get<S>(tpl) = value.*LastRef::pointer_value;
        }
    };

public:
    element_proxy(soa_detail::member_type_t<PointerTypes>&... args)
        : pointers_(&args...)
    {
    }

    operator T()
    {
        return make_dispatch(std::index_sequence_for<PointerTypes...>());
    }

    element_proxy&
    operator=(const T& value)
    {
        assignment_helper<member_reference<PointerTypes, PointerValues>...>::
            assign(
                pointers_, value, std::index_sequence_for<PointerTypes...>());

        return *this;
    }

private:
    std::tuple<soa_detail::member_type_t<PointerTypes>*...> pointers_;
};


// helper to access respective parents that are multiply inherited
template <class FirstParent, class... RestParents>
struct parent_helper
{
    template <class SoaType>
    static std::size_t
    size(const SoaType& s)
    {
        return s.FirstParent::members_.size();
    }

    template <class SoaType>
    static void
    push_back(SoaType& s, const typename SoaType::value_type& value)
    {
        s.FirstParent::members_.push_back(value.*FirstParent::pointer);
        parent_helper<RestParents...>::push_back(s, value);
    }
};


template <class LastParent>
struct parent_helper<LastParent>
{
    template <class SoaType>
    static std::size_t
    size(const SoaType& s)
    {
        return s.LastParent::members_.size();
    }

    template <class SoaType>
    static void
    push_back(SoaType& s, const typename SoaType::value_type& value)
    {
        s.LastParent::members_.push_back(value.*LastParent::pointer);
    }
};


template <class MemberPointerType, MemberPointerType MemberPointerValue>
class member_container
{
public:
    typedef soa_detail::member_type_t<MemberPointerType> member_type;
    typedef soa_detail::object_type_t<MemberPointerType> object_type;

    static constexpr MemberPointerType pointer = MemberPointerValue;

public:
    member_container() = default;

    std::vector<member_type> members_;
};


template <class T, class... MemberContainer>
class soa;

template <class T, class... MemberPtrTypes, MemberPtrTypes... MemberPtrValues>
class soa<T, member_container<MemberPtrTypes, MemberPtrValues>...>
    : public member_container<MemberPtrTypes, MemberPtrValues>...
{
public:
    typedef T value_type;

public:
    soa() : member_container<MemberPtrTypes, MemberPtrValues>()...
    {
    }


    std::size_t
    size() const
    {
        return parent_helper<
            member_container<MemberPtrTypes, MemberPtrValues>...>::size(*this);
    }

    void
    push_back(const T& value)
    {
        parent_helper<member_container<MemberPtrTypes, MemberPtrValues>...>::
            push_back(*this, value);
    }
};
}

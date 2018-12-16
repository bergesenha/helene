#pragma once

#include <iostream>


namespace helene
{
namespace static_json
{

enum class value_type
{
    number_type,
    bool_type,
    string_type,
    null_type,
    array_type,
    object_type
};


template <class NameProvider, class Type>
struct field
{
};


template <class T>
struct holder
{
    T value;
};


template <class... Fields>
class json;

template <class... NameProviders, class... Types>
class json<field<NameProviders, Types>...> : holder<Types>...
{
public:
private:
};
} // namespace static_json
} // namespace helene

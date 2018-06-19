#pragma once

#include "stable_container.hpp"

#include <type_list.hpp>


namespace helene
{
template <class TableTagType, TableTagType TableTag, class T>
struct table_description
{
};


namespace detail
{
template <class T>
class container;


template <class TableTagType, TableTagType TableTag, class T>
class container<table_description<TableTagType, TableTag, T>>
{
protected:
    stable_vector<T> data_;
};
} // namespace detail


template <class... TableTags>
class database : public detail::container<TableTags>...
{
public:
private:
};
} // namespace helene

#pragma once

#include <tuple>

#include "handle_map.hpp"


namespace helene
{
template <class LabelType, LabelType Label, class T>
struct column_description
{
};


namespace detail
{
template <class LabelType, LabelType Label, class... ColumnDescriptions>
struct type_by_label;

template <class LabelType,
          LabelType Label,
          LabelType FirstLabel,
          LabelType... RestLabels,
          class FirstType,
          class... RestTypes>
struct type_by_label<LabelType,
                     Label,
                     column_description<LabelType, FirstLabel, FirstType>,
                     column_description<LabelType, RestLabels, RestTypes>...>
    : type_by_label<LabelType,
                    Label,
                    column_description<LabelType, RestLabels, RestTypes>...>
{
};


template <class LabelType,
          LabelType Label,
          LabelType... RestLabels,
          class FirstType,
          class... RestTypes>
struct type_by_label<LabelType,
                     Label,
                     column_description<LabelType, Label, FirstType>,
                     column_description<LabelType, RestLabels, RestTypes>...>
{
    typedef FirstType type;
};

template <class LabelType, LabelType Label, class... ColumnDescriptions>
using type_by_label_t =
    typename type_by_label<LabelType, Label, ColumnDescriptions...>::type;
}; // namespace detail


template <class ColumnDescription>
class column;

template <class LabelType, LabelType Label, class T>
class column<column_description<LabelType, Label, T>>
{
protected:
    typename handle_map<T>::handle_type
    erase(typename handle_map<T>::handle_type n)
    {
        data_.erase(n);
        return 0;
    }

protected:
    handle_map<T> data_;
};


template <class LabelType, class... ColumnDescriptions>
class table;

template <class LabelType, LabelType... Labels, class... Ts>
class table<LabelType, column_description<LabelType, Labels, Ts>...>
    : public column<column_description<LabelType, Labels, Ts>>...
{
    template <LabelType Label>
    using get_type_t =
        detail::type_by_label_t<LabelType,
                                Label,
                                column_description<LabelType, Labels, Ts>...>;

    template <LabelType Label>
    using column_type =
        column<column_description<LabelType, Label, get_type_t<Label>>>;

public:
    typedef std::size_t row_index_type;
    typedef std::size_t size_type;

    template <LabelType Label>
    using iterator = typename handle_map<get_type_t<Label>>::iterator;

    template <LabelType Label>
    using const_iterator =
        typename handle_map<get_type_t<Label>>::const_iterator;


public:
    row_index_type
    insert_row(const Ts&... elms)
    {
        row_index_type dummy[] = {column_type<Labels>::data_.insert(elms)...};

        return dummy[0];
    }

    void
    erase_row(row_index_type n)
    {
        auto dummy = {column_type<Labels>::erase(n)...};
    }

    template <LabelType Label>
    get_type_t<Label>&
    get(row_index_type n)
    {
        return column_type<Label>::data_[n];
    }

    template <LabelType Label>
    const get_type_t<Label>&
    get(row_index_type n) const
    {
        return column_type<Label>::data_[n];
    }

    size_type
    size() const
    {
        size_type dummy[] = {column_type<Labels>::data_.size()...};

        return dummy[0];
    }

    bool
    empty() const
    {
        bool dummy[] = {column_type<Labels>::data_.empty()...};

        return dummy[0];
    }

    template <LabelType Label>
    iterator<Label>
    column_begin()
    {
        return column_type<Label>::data_.begin();
    }

    template <LabelType Label>
    iterator<Label>
    column_end()
    {
        return column_type<Label>::data_.end();
    }
};

} // namespace helene

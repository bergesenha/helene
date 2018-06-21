#pragma once

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
}; // namespace detail


template <class ColumnDescription>
class column;

template <class LabelType, LabelType Label, class T>
class column<column_description<LabelType, Label, T>>
{
protected:
    handle_map<T> data_;
};


template <class... ColumnDescriptions>
class table : public column<ColumnDescriptions>...
{
public:
private:
};

} // namespace helene

#pragma once

#include "stable_container.hpp"

#include <type_list.hpp>


namespace helene
{
template <class TagType, TagType Tag, class T>
struct column_description
{
};

template <class TagType, TagType Tag, class... TableDescriptions>
struct type_from_tag;

template <class TagType,
          TagType Tag,
          TagType FirstTag,
          TagType... RestTags,
          class FirstT,
          class... RestTs>
struct type_from_tag<TagType,
                     Tag,
                     column_description<TagType, FirstTag, FirstT>,
                     column_description<TagType, RestTags, RestTs>...>
    : type_from_tag<TagType,
                    Tag,
                    column_description<TagType, RestTags, RestTs>...>
{
};

template <class TagType,
          TagType Tag,
          TagType... RestTags,
          class FirstT,
          class... RestTs>
struct type_from_tag<TagType,
                     Tag,
                     column_description<TagType, Tag, FirstT>,
                     column_description<TagType, RestTags, RestTs>...>
{
    typedef FirstT type;
};

template <class TagType, TagType Tag, class... TableDescriptions>
using type_from_tag_t =
    typename type_from_tag<TagType, Tag, TableDescriptions...>::type;


template <class TagType, TagType Tag, class T>
class container
{
protected:
    stable_vector<T> data_;
};


template <class TagType, class... TableDescriptions>
class table;


template <class TagType, TagType... Tags, class... Ts>
class table<TagType, column_description<TagType, Tags, Ts>...>
    : public container<TagType, Tags, Ts>...
{
public:
    typedef std::size_t index_type;

public:
    template <TagType Tag>
    index_type
    insert(
        type_from_tag_t<TagType, Tag, column_description<TagType, Tags, Ts>...>
            value)
    {
        return container<TagType,
                         Tag,
                         type_from_tag_t<
                             TagType,
                             Tag,
                             column_description<TagType, Tags, Ts>...>>::data_
            .add(value);
    }

private:
};
} // namespace helene

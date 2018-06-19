#pragma once

#include "stable_container.hpp"

#include <type_list.hpp>


namespace helene
{
template <class TagType, TagType Tag, class T>
struct table_description
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
                     table_description<TagType, FirstTag, FirstT>,
                     table_description<TagType, RestTags, RestTs>...>
    : type_from_tag<TagType,
                    Tag,
                    table_description<TagType, RestTags, RestTs>...>
{
};

template <class TagType,
          TagType Tag,
          TagType... RestTags,
          class FirstT,
          class... RestTs>
struct type_from_tag<TagType,
                     Tag,
                     table_description<TagType, Tag, FirstT>,
                     table_description<TagType, RestTags, RestTs>...>
{
    typedef FirstT type;
};

template <class TagType, class... TableDescriptions>
class database;

template <class TagType, TagType... Tags, class... Ts>
class database<TagType, table_description<TagType, Tags, Ts>...>
{
};
} // namespace helene

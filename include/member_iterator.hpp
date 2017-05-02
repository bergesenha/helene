#pragma once

#include <iterator>


namespace helene
{


template <class MemberType,
          class StructType,
          template <class...> class Container,
          MemberType StructType::*PtrValue>
class member_iterator;
}

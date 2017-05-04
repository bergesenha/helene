# Helene

Helene is a small collection of classes and algorithms that I have found useful
over time.

## member_iterator

Sometimes when implementing a custom data structure the need arises for
embedding a user facing type in structs/classes of some other data structure.
For example the struct/class that the user facing type is embedded in may carry
additional implementation specific information such as "head" and "tail" in a
list implementation. In general, this extra information is of no interest to the
user of this custom data structure, so we desire to expose only the user facing
objects.

**member_iterator** is a generic wrapper around any iterator, masquerading as an
iterator to a member of the struct the underlying iterator is referencing.

```c++
#include <vector>
#include <algorithm>
#include <type_traits>
#include <iterator>
#include <member_iterator.hpp>


struct impl_struct
{
    size_t metadata;
    double payload;
};

std::vector<impl_struct> impl_data{{0, 4.4}, {2, 3.1}, {1, 2.6}};


typedef helene::member_iterator<
    double,                             // type of member
    impl_struct,                        // type of struct
    std::vector<impl_struct>::iterator, // type of iterator to struct
    &impl_struct::payload>              // pointer to member
    my_payload_iterator;

// constructor takes an underlying iterator
my_payload_iterator payload_begin(impl_data.begin());
my_payload_iterator payload_end(impl_data.end());


// behaves just like a normal iterator
double d = *payload_begin; // d is 4.4

auto found = std::find(payload_begin, payload_end, 3.1);

d = *found; // d is 3.1

// member_iterator takes on capabilities according to underlying iterator
// category
// in this case RandomAccesIterator
static_assert(
    std::is_same<std::iterator_traits<my_payload_iterator>::iterator_category,
                 std::iterator_traits<std::vector<impl_struct>::iterator>::iterator_category>::value, "")
std::size_t index = found - payload_begin; // index is 1


// cast back to underlying iterator
auto underlying_found = helene::cast_to_struct_iterator(found);

d = underlying_found->payload; // d is 3.1
size_t metadata_of_found = underlying_found->metadata; // metadata_of_found is 2

```

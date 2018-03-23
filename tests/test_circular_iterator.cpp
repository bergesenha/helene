#include "catch.hpp"
#include <vector>

#include <circular_iterator.hpp>


TEST_CASE("test is_iterator_of_category_v for different categories",
          "[detail::is_iterator_of_category_v]")
{
    typedef std::vector<int>::iterator vec_it;

    constexpr bool vector_it_is_random_access = helene::detail::
        is_iterator_of_category_v<vec_it, std::random_access_iterator_tag>;

    constexpr bool vector_it_is_forward_iterator =
        helene::detail::is_iterator_of_category_v<vec_it,
                                                  std::forward_iterator_tag>;

    CHECK(vector_it_is_random_access);
    CHECK(!vector_it_is_forward_iterator);
}

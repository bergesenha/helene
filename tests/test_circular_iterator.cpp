#include "catch.hpp"
#include <vector>
#include <forward_list>

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


TEST_CASE("test is_at_least_iterator_of_category_v for different categories",
          "[detail::is_at_least_iterator_of_category_v]")
{
    typedef std::vector<int>::iterator vec_it;
    typedef std::forward_list<int>::iterator fl_it;

    constexpr bool vec_it_at_least_forward = helene::detail::
        is_at_least_iterator_of_category_v<vec_it, std::forward_iterator_tag>;

    constexpr bool vec_it_at_least_bidirectional =
        helene::detail::is_at_least_iterator_of_category_v<
            vec_it,
            std::bidirectional_iterator_tag>;

    constexpr bool fl_it_not_at_least_random =
        !helene::detail::is_at_least_iterator_of_category_v<
            fl_it,
            std::random_access_iterator_tag>;

    CHECK(vec_it_at_least_forward);
    CHECK(vec_it_at_least_bidirectional);
    CHECK(fl_it_not_at_least_random);
}


TEST_CASE("default construct a circular_iterator", "[circular_iterator]")
{
    helene::circular_iterator<std::forward_list<int>::iterator> ci1;
}


TEST_CASE("construct a circular_iterator to a forward_list",
          "[circular_iterator]")
{
    std::forward_list<int> mylist{1, 2, 3};

    helene::circular_iterator<std::forward_list<int>::iterator> cit1(
        mylist.begin(), mylist.end(), mylist.begin());

    CHECK(*cit1 == 1);

    SECTION("pre increment")
    {
        ++cit1;

        CHECK(*cit1 == 2);

        SECTION("post increment")
        {
            auto t = *cit1++;
            CHECK(*cit1 == 3);
            CHECK(t == 2);

            SECTION("pre increment")
            {
                ++cit1;
                CHECK(*cit1 == 1);
            }

            SECTION("post increment")
            {
                auto t2 = *cit1++;
                CHECK(*cit1 == 1);
                CHECK(t2 == 3);
            }
        }
    }
}

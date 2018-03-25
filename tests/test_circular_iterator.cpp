#include "catch.hpp"
#include <vector>
#include <list>
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
    typedef helene::circular_iterator<std::forward_list<int>::iterator>
        cil_type;

    std::forward_list<int> mylist{1, 2, 3};

    cil_type cit1(mylist.begin(), mylist.end(), mylist.begin());

    constexpr bool is_forward_iterator =
        helene::detail::is_iterator_of_category_v<cil_type,
                                                  std::forward_iterator_tag>;

    CHECK(*cit1 == 1);
    CHECK(is_forward_iterator);

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

    SECTION("copy iterator")
    {
        auto cit2 = cit1;

        CHECK(cit1 == cit2);

        SECTION("increment first")
        {
            ++cit1;

            CHECK(cit1 != cit2);
        }
    }
}

TEST_CASE("construct a circular_iterator pointing to a list",
          "[circular_iterator]")
{
    typedef helene::circular_iterator<std::list<int>::iterator> cil_type;

    constexpr bool is_bidirectional = helene::detail::
        is_iterator_of_category_v<cil_type, std::bidirectional_iterator_tag>;

    CHECK(is_bidirectional);

    std::list<int> l{1, 2, 3};

    cil_type cit1{l.begin(), l.end(), l.begin()};

    CHECK(*cit1 == 1);

    SECTION("preincrement")
    {
        auto it = ++cit1;

        CHECK(*it == 2);
        CHECK(*cit1 == 2);
        CHECK(it == cit1);
    }

    SECTION("predecrement")
    {
        auto it = --cit1;

        CHECK(*it == 3);
        CHECK(*cit1 == 3);
        CHECK(it == cit1);
    }

    SECTION("postdecrement")
    {
        auto it = cit1--;

        CHECK(*it == 1);
        CHECK(*cit1 == 3);
        CHECK(it != cit1);
    }
}

TEST_CASE("construct a circular_iterator pointing to a vector",
          "[circular_iterator]")
{
    typedef helene::circular_iterator<std::vector<int>::iterator> civ_type;

    constexpr bool is_random_access = helene::detail::
        is_iterator_of_category_v<civ_type, std::random_access_iterator_tag>;

    std::vector<int> v{1, 2, 3};

    civ_type cit1{v.begin(), v.end(), v.begin()};

    CHECK(*cit1 == 1);
    CHECK(is_random_access);

    SECTION("preincrement iterator")
    {
        auto it = ++cit1;
        CHECK(*it == 2);
        CHECK(*cit1 == 2);
        CHECK(it == cit1);
    }

    SECTION("postincrement iterator")
    {
        auto it = cit1++;
        CHECK(*it == 1);
        CHECK(*cit1 == 2);
        CHECK(it != cit1);
    }

    SECTION("assign-increment by 2")
    {
        cit1 += 2;

        CHECK(*cit1 == 3);
    }

    SECTION("assign-increment by 4")
    {
        cit1 += 4;

        CHECK(*cit1 == 2);
    }

    SECTION("assign-increment by 10")
    {
        cit1 += 10;

        CHECK(*cit1 == 2);
    }

    SECTION("assign-increment by -2")
    {
        cit1 += -2;

        CHECK(*cit1 == 2);
    }

    SECTION("assign-increment by -10")
    {
        cit1 += -10;

        CHECK(*cit1 == 3);
    }
}

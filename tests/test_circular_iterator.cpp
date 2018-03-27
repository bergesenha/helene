#include "catch.hpp"
#include <vector>
#include <list>
#include <forward_list>

#include <circular_iterator.hpp>


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

    CHECK(cit1[0] == 1);
    CHECK(cit1[1] == 2);
    CHECK(cit1[2] == 3);
    CHECK(cit1[3] == 1);
    CHECK(cit1[10] == 2);
    CHECK(cit1[-1] == 3);
    CHECK(cit1[-8] == 2);


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

    SECTION("assign-decrement by 3")
    {
        cit1 -= 3;

        CHECK(*cit1 == 1);
    }

    SECTION("assign-decrement by 13")
    {
        cit1 -= 13;

        CHECK(*cit1 == 3);
    }

    SECTION("assign-decrement by -5")
    {
        cit1 -= -5;

        CHECK(*cit1 == 3);
    }

    SECTION("add 2 to iterator")
    {
        auto it = cit1 + 2;

        CHECK(*it == 3);

        CHECK(cit1 < it);
        CHECK((cit1 - 2) < (cit1 + 2));
    }

    SECTION("add 10 to iterator")
    {
        auto it = cit1 + 10;

        CHECK(*it == 2);
    }

    SECTION("add -1 to iterator")
    {
        auto it = cit1 + -1;

        CHECK(*it == 3);
    }

    SECTION("add -10 to iterator")
    {
        auto it = cit1 + -10;

        CHECK(*it == 3);
    }

    SECTION("add iterator to 2")
    {
        auto it = 2 + cit1;

        CHECK(*it == 3);
    }


    SECTION("subtract 5 from iterator")
    {
        auto it = cit1 - 5;

        CHECK(*it == 2);
    }

    SECTION("modify first element through iterator")
    {
        *cit1 = 10;

        CHECK(*cit1 == 10);
        CHECK(v.front() == 10);
    }
}


TEST_CASE("create circular_iterator with underlying iterator being vector "
          "const_iterator",
          "[circular_iterator]")
{
    typedef helene::circular_iterator<std::vector<int>::const_iterator>
        civ_type;

    const std::vector<int> v{1, 2, 3};

    civ_type cit1(v.cbegin(), v.cend(), v.cbegin());

    CHECK(*cit1 == 1);

    CHECK(cit1[0] == 1);
    CHECK(cit1[1] == 2);
    CHECK(cit1[2] == 3);
    CHECK(cit1[3] == 1);
    CHECK(cit1[10] == 2);
    CHECK(cit1[-1] == 3);
    CHECK(cit1[-8] == 2);


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

    SECTION("assign-decrement by 3")
    {
        cit1 -= 3;

        CHECK(*cit1 == 1);
    }

    SECTION("assign-decrement by 13")
    {
        cit1 -= 13;

        CHECK(*cit1 == 3);
    }

    SECTION("assign-decrement by -5")
    {
        cit1 -= -5;

        CHECK(*cit1 == 3);
    }

    SECTION("add 2 to iterator")
    {
        auto it = cit1 + 2;

        CHECK(*it == 3);

        CHECK(cit1 < it);
        CHECK((cit1 - 2) < (cit1 + 2));
    }

    SECTION("add 10 to iterator")
    {
        auto it = cit1 + 10;

        CHECK(*it == 2);
    }

    SECTION("add -1 to iterator")
    {
        auto it = cit1 + -1;

        CHECK(*it == 3);
    }

    SECTION("add -10 to iterator")
    {
        auto it = cit1 + -10;

        CHECK(*it == 3);
    }

    SECTION("add iterator to 2")
    {
        auto it = 2 + cit1;

        CHECK(*it == 3);
    }


    SECTION("subtract 5 from iterator")
    {
        auto it = cit1 - 5;

        CHECK(*it == 2);
    }
}

#include "catch.hpp"

#include <member_iterator.hpp>

#include <forward_list>


TEST_CASE("member_iterator based on ForwardIterator", "[member_iterator]")
{
    struct mock
    {
        char c;
        int i;
    };

    std::forward_list<mock> mocklist{{'a', 1}, {'b', 2}, {'c', 3}};

    typedef helene::
        member_iterator<int, mock, std::forward_list<mock>::iterator, &mock::i>
            mocklist_iterator_type;

    mocklist_iterator_type beg_it = mocklist.begin();
    mocklist_iterator_type end_it = mocklist.end();

    SECTION("construction")
    {
        auto elem = *beg_it;
        mocklist_iterator_type cop_it = beg_it;
        mocklist_iterator_type mov_it = std::move(beg_it);

        REQUIRE(*mov_it == elem);
        REQUIRE(cop_it == mov_it);
    }

    SECTION("assignment")
    {
        mocklist_iterator_type cop_it = beg_it;
        cop_it = end_it;

        REQUIRE(cop_it == end_it);
    }

    SECTION("move assignment")
    {
        mocklist_iterator_type an_it = beg_it;
        mocklist_iterator_type another_it = end_it;

        another_it = std::move(an_it);

        REQUIRE(another_it == beg_it);
        REQUIRE(*another_it == *beg_it);
    }
}

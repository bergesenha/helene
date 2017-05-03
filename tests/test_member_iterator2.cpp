#include "catch.hpp"

#include <utility>
#include <type_traits>
#include <iterator>

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

    SECTION("swappable")
    {
        using std::swap;

        mocklist_iterator_type beg_it2 = beg_it;
        mocklist_iterator_type end_it2 = end_it;

        swap(beg_it2, end_it2);

        REQUIRE(beg_it2 == end_it);
        REQUIRE(end_it2 == beg_it);
    }

    SECTION("traits")
    {
        std::iterator_traits<mocklist_iterator_type>::value_type i = 20;
        std::iterator_traits<mocklist_iterator_type>::difference_type j = -2;
        std::iterator_traits<mocklist_iterator_type>::reference k = i;
        std::iterator_traits<mocklist_iterator_type>::pointer l = &i;

        bool is_forward_iterator = std::is_same<
            std::iterator_traits<mocklist_iterator_type>::iterator_category,
            std::forward_iterator_tag>::value;

        REQUIRE(is_forward_iterator == true);
    }

    SECTION("dereference")
    {
        auto is_reference = std::is_same<decltype(*beg_it), int&>::value;

        REQUIRE(is_reference == true);
        REQUIRE(*beg_it == 1);
    }

    SECTION("preincrement")
    {
        auto ref_it = ++beg_it;

        bool is_iterator_reference =
            std::is_same<decltype(++beg_it), mocklist_iterator_type&>::value;

        REQUIRE(ref_it == beg_it);
        REQUIRE(*beg_it == 2);
        REQUIRE(is_iterator_reference == true);
    }
}

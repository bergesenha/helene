#include "catch.hpp"

#include <utility>
#include <type_traits>
#include <iterator>
#include <algorithm>

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

    ////////////////////////////////////////////////////////////////////////////
    // Iterator concept
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

    SECTION("EqualityComparable")
    {
        bool equal = beg_it == end_it;


        REQUIRE(equal == false);
        REQUIRE(beg_it == mocklist.begin());
    }

    ////////////////////////////////////////////////////////////////////////////
    // InputIterator
    SECTION("!=")
    {
        bool not_equal = beg_it != mocklist.begin();

        REQUIRE(beg_it != end_it);
        REQUIRE(not_equal == false);
    }

    SECTION("postincrement")
    {
        bool is_conv_to_const_ref =
            std::is_convertible<decltype(beg_it++),
                                const mocklist_iterator_type&>::value;

        (void)beg_it++;

        REQUIRE(is_conv_to_const_ref == true);
        REQUIRE(*beg_it == 2);
    }

    SECTION("postincrement and dereference")
    {
        bool is_conv_to_value_type =
            std::is_convertible<decltype(*beg_it++), int>::value;

        auto val = *beg_it++;

        REQUIRE(is_conv_to_value_type == true);
        REQUIRE(val == 1);
        REQUIRE(*beg_it == 2);
    }

    ////////////////////////////////////////////////////////////////////////////
    // OutputIterator
    SECTION("assignment through iterator")
    {
        *beg_it = 10;

        REQUIRE(*beg_it == 10);
        REQUIRE(mocklist.front().i == 10);
        REQUIRE(mocklist.front().c == 'a');
    }


    ////////////////////////////////////////////////////////////////////////////
    // ForwardIterator
    SECTION("DefaultConstructible")
    {
        mocklist_iterator_type it;
        mocklist_iterator_type it2{};
        mocklist_iterator_type();
        mocklist_iterator_type{};
    }

    SECTION("multipass guarantee")
    {
        auto beg_it_copy = beg_it;
        ++beg_it;
        ++beg_it;

        auto val0 = *beg_it_copy++;
        auto val1 = *beg_it_copy++;
        auto val2 = *beg_it_copy;

        REQUIRE(val0 == 1);
        REQUIRE(val1 == 2);
        REQUIRE(val2 == 3);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Test std algorithms
    SECTION("std::equal")
    {
        std::forward_list<mock> mocklist_copy = mocklist;

        mocklist_iterator_type beg_it_copy(mocklist_copy.begin());

        REQUIRE(std::equal(beg_it, end_it, beg_it_copy) == true);
    }

    SECTION("std::find")
    {
        auto found = std::find(beg_it, end_it, 2);

        REQUIRE(found != end_it);
    }
}

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

    SECTION("std::for_each")
    {
        int sum = 0;

        std::for_each(beg_it, end_it, [&sum](int i) { sum += i; });

        REQUIRE(sum == 6);
    }

    SECTION("std::search") // requires ForwardIterator
    {
        std::vector<int> subsequence;
        subsequence.push_back(2);
        subsequence.push_back(3);

        auto found =
            std::search(beg_it, end_it, subsequence.begin(), subsequence.end());

        REQUIRE(found != end_it);
    }

    SECTION("std::remove") // requires ForwardIterator
    {
        auto past_end = std::remove(beg_it, end_it, 2);
        auto native_iter = mocklist.begin();

        REQUIRE(*beg_it == 1);
        REQUIRE(native_iter->i == 1);
        REQUIRE(native_iter->c == 'a');

        SECTION("increment to second element")
        {
            ++beg_it;
            ++native_iter;

            REQUIRE(*beg_it == 3);
            REQUIRE(native_iter->i == 3);
            REQUIRE(native_iter->c == 'b');
        }
    }
}


TEST_CASE("member_iterator based on std::vector::iterator", "[member_iterator]")
{
    struct payload
    {
        int i;
        int
        get_i() const
        {
            return i;
        }

        bool
        operator==(const payload& other) const
        {
            return i == other.i;
        }
    };

    struct mock
    {
        double weight;
        payload value;
    };

    typedef helene::member_iterator<payload,
                                    mock,
                                    std::vector<mock>::iterator,
                                    &mock::value>
        mockveck_iterator_type;

    std::vector<mock> mockvec{{0.3, {1}}, {0.4, {2}}, {0.3, {3}}};

    mockveck_iterator_type beg_it(mockvec.begin());
    mockveck_iterator_type end_it(mockvec.end());

    SECTION("category should be random_access_iterator_tag")
    {
        bool is_rai = std::is_same<
            std::iterator_traits<mockveck_iterator_type>::iterator_category,
            std::random_access_iterator_tag>::value;

        REQUIRE(is_rai == true);
    }

    ////////////////////////////////////////////////////////////////////////////
    // BidirectionalIterator

    SECTION("predecrement")
    {
        --end_it;

        REQUIRE(end_it->i == 3);
        REQUIRE(end_it->get_i() == 3);

        SECTION("cast back to vector::iterator")
        {
            // called with namespace
            auto vec_it = helene::cast_to_struct_iterator(end_it);

            // called through ADL
            auto vec_it2 = cast_to_struct_iterator(beg_it);

            REQUIRE(vec_it->weight == 0.3);
            REQUIRE(vec_it2->value.i == 1);
        }
    }

    SECTION("postdecrement")
    {
        end_it--;

        REQUIRE(end_it->i == 3);
        REQUIRE(end_it->get_i() == 3);
    }

    SECTION("postdecrement and dereference statement")
    {
        ++beg_it;

        auto old = *beg_it--;
        REQUIRE(old.i == 2);
        REQUIRE(beg_it->i == 1);
    }

    ////////////////////////////////////////////////////////////////////////////
    // RandomAccesIterator

    SECTION("+=")
    {
        bool is_iter_ref =
            std::is_same<decltype(beg_it += 2), mockveck_iterator_type&>::value;

        beg_it += 2;

        REQUIRE(is_iter_ref == true);

        REQUIRE(beg_it->i == 3);
    }

    SECTION("adding number")
    {
        bool is_iter_type =
            std::is_same<decltype(beg_it + 2), mockveck_iterator_type>::value;
        bool is_iter_type2 =
            std::is_same<decltype(2 + beg_it), mockveck_iterator_type>::value;

        auto res1 = beg_it + 2;
        auto res2 = 2 + beg_it;

        REQUIRE(is_iter_type == true);
        REQUIRE(is_iter_type2 == true);

        REQUIRE(res1 == res2);
        REQUIRE(res1->i == 3);
        REQUIRE(res2->i == 3);
    }

    SECTION("-=")
    {
        bool is_iter_ref =
            std::is_same<decltype(end_it -= 2), mockveck_iterator_type&>::value;

        end_it -= 2;

        REQUIRE(is_iter_ref == true);
        REQUIRE(end_it->i == 2);
    }

    SECTION("subtracting number")
    {
        bool is_iter_type =
            std::is_same<decltype(end_it - 2), mockveck_iterator_type>::value;

        auto res1 = end_it - 3;

        REQUIRE(is_iter_type == true);
        REQUIRE(res1->i == 1);
    }

    SECTION("subtracting two iterators")
    {
        bool is_difference_type =
            std::is_same<decltype(end_it - beg_it),
                         std::iterator_traits<
                             mockveck_iterator_type>::difference_type>::value;

        REQUIRE(end_it - beg_it == mockvec.size());
    }

    SECTION("array_access")
    {
        REQUIRE(beg_it[0].i == 1);
        REQUIRE(beg_it[2].i == 3);
        REQUIRE(end_it[-1].i == 3);
    }

    SECTION("less than")
    {

        REQUIRE(beg_it < end_it);
        REQUIRE(beg_it <= end_it);
        REQUIRE(beg_it <= beg_it);
    }

    SECTION("greater than")
    {
        REQUIRE(end_it > beg_it);
        REQUIRE(end_it >= beg_it);
        REQUIRE(end_it >= end_it);
    }

    ////////////////////////////////////////////////////////////////////////////
    // std library algorithm tests

    SECTION("std::equal")
    {
        std::vector<payload> payloads{{1}, {2}, {3}};

        REQUIRE(std::equal(beg_it, end_it, payloads.begin()));
    }

    SECTION("std::find")
    {
        auto found = std::find(beg_it, end_it, payload{2});

        REQUIRE(found != end_it);
        REQUIRE(found->i == 2);
    }

    SECTION("std::find_if")
    {
        auto found = std::find_if(beg_it, end_it, [](const payload& pl) {
            if(pl.get_i() == 3)
            {
                return true;
            }
            else
            {
                return false;
            }
        });

        REQUIRE(found != end_it);
        REQUIRE(found->i == 3);
    }

    // requires RandomAccessIterator
    SECTION("std::sort")
    {
        // reverse sort
        std::sort(beg_it, end_it, [](const payload& lhs, const payload& rhs) {
            if(lhs.i > rhs.i)
            {
                return true;
            }
            else
            {
                return false;
            }
        });

        auto second = beg_it + 1;
        auto third = beg_it + 2;

        REQUIRE(beg_it->i == 3);
        REQUIRE(second->i == 2);
        REQUIRE(third->i == 1);

        // rest of mock structs untouched
        REQUIRE(mockvec[0].value.i == 3);
        REQUIRE(mockvec[0].weight == 0.3);
        REQUIRE(mockvec[1].value.i == 2);
        REQUIRE(mockvec[1].weight == 0.4);
        REQUIRE(mockvec[2].value.i == 1);
        REQUIRE(mockvec[2].weight == 0.3);
    }
}

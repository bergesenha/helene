#include "catch.hpp"


#include <member_iterator.hpp>
#include <vector>
#include <forward_list>
#include <list>
#include <string>


TEST_CASE("member_iterator to member of a mock struct in std::vector",
          "[member_iterator]")
{


    struct mock
    {
        int something;
        double the_member;
    };

    typedef helene::member_iterator<double,
                                    mock,
                                    std::vector<mock>::iterator,
                                    &mock::the_member>
        my_iter;

    std::vector<mock> container{{1, 1.0}, {2, 2.0}, {3, 3.0}};

    my_iter beg_it(container.begin());
    my_iter end_it(container.end());


    SECTION("operator+= 2")
    {
        beg_it += 2;

        REQUIRE(*beg_it == 3.0);
    }

    SECTION("operator+ 2")
    {
        auto new_it = beg_it + 2;

        REQUIRE(*new_it == 3.0);
        REQUIRE(*beg_it == 1.0);
    }
}


TEST_CASE("member_iterator to member of a mock struct in a std::forward_list",
          "[member_iterator]")
{
    struct mock
    {
        std::string name;
        int value;
    };

    typedef helene::member_iterator<int,
                                    mock,
                                    std::forward_list<mock>::iterator,
                                    &mock::value>
        my_iter;

    std::forward_list<mock> container;
    container.push_front({std::string("one"), 1});
    container.push_front({std::string("two"), 2});
    container.push_front({std::string("three"), 3});

    my_iter it_beg(container.begin());
    my_iter it_end(container.end());

    REQUIRE(it_beg != it_end);
    REQUIRE(*it_beg == 3);

    SECTION("preincrement iterator")
    {
        ++it_beg;
        REQUIRE(*it_beg == 2);

        SECTION("preincrement again")
        {
            ++it_beg;
            REQUIRE(*it_beg == 1);

            SECTION("preincrement again")
            {
                ++it_beg;
                REQUIRE_FALSE(it_beg != it_end);
            }
        }
    }

    SECTION("preincrement and dereference")
    {
        REQUIRE(*(++it_beg) == 2);
    }

    SECTION("postincrement")
    {
        auto prev = it_beg++;
        REQUIRE(*prev == 3);
        REQUIRE(*it_beg == 2);
    }

    SECTION("mutate element through iterator")
    {
        *it_beg = 10;

        REQUIRE(*it_beg == 10);
    }
}

TEST_CASE("member_iterator to member of a mock struct in a std::list",
          "[member_iterator]")
{
    struct mock
    {
        std::string name;
        int value;
    };

    typedef helene::
        member_iterator<int, mock, std::list<mock>::iterator, &mock::value>
            my_iter;

    std::list<mock> container{{std::string("one"), 1},
                              {std::string("two"), 2},
                              {std::string("three"), 3}};


    my_iter it_beg(container.begin());
    my_iter it_end(container.end());

    REQUIRE(*it_beg == 1);

    SECTION("increment iterator")
    {
        ++it_beg;

        REQUIRE(*it_beg == 2);

        SECTION("predecrement")
        {
            --it_beg;

            REQUIRE(*it_beg == 1);
        }

        SECTION("post decrement")
        {
            auto prev = it_beg--;

            REQUIRE(*it_beg == 1);
            REQUIRE(*prev == 2);
        }

        SECTION("post decrement and dereference")
        {
            auto prev_val = *it_beg--;

            REQUIRE(*it_beg == 1);
            REQUIRE(prev_val == 2);
        }
    }
}

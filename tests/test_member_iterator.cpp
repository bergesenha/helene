#include "catch.hpp"


#include <member_iterator.hpp>
#include <vector>
#include <forward_list>
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

    SECTION("increment iterator")
    {
        ++it_beg;
        REQUIRE(*it_beg == 2);

        SECTION("increment again")
        {
            ++it_beg;
            REQUIRE(*it_beg == 1);

            SECTION("increment again")
            {
                ++it_beg;
                REQUIRE_FALSE(it_beg != it_end);
            }
        }
    }

    SECTION("mutate element through iterator")
    {
        *it_beg = 10;

        REQUIRE(*it_beg == 10);
    }
}

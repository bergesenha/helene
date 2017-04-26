#include "catch.hpp"


#include <dag.hpp>
#include <utility>


TEST_CASE("persistent_iterator_ to mock vector", "[presistent_iterator_]")
{

    std::vector<char> mock_nodes;
    mock_nodes.push_back('a');

    helene::persistent_iterator_<char, int> my_iter(0, mock_nodes);

    REQUIRE(*my_iter == 'a');
    REQUIRE(my_iter[0] == 'a');


    SECTION("copy construct iterator")
    {
        helene::persistent_iterator_<char, int> my_iter2 = my_iter;

        REQUIRE(*my_iter2 == 'a');
        REQUIRE(my_iter == my_iter2);

        REQUIRE(my_iter <= my_iter2);
        REQUIRE(my_iter >= my_iter2);
    }

    SECTION("move construct iterator")
    {
        helene::persistent_iterator_<char, int> my_iter2(std::move(my_iter));

        REQUIRE(*my_iter2 == 'a');
    }

    SECTION("copy assign iterator")
    {
        helene::persistent_iterator_<char, int> my_iter2(10, mock_nodes);

        my_iter2 = my_iter;

        REQUIRE(*my_iter2 == 'a');
        REQUIRE(my_iter == my_iter2);
    }

    SECTION("add another element in mock vector")
    {
        mock_nodes.push_back('b');

        REQUIRE(*my_iter == 'a');
        REQUIRE(my_iter[1] == 'b');

        SECTION("preincrement iterator")
        {
            ++my_iter;

            REQUIRE(*my_iter == 'b');
            REQUIRE(my_iter[-1] == 'a');
        }

        SECTION("posincrement iterator")
        {
            auto old = my_iter++;

            REQUIRE(*old == 'a');
            REQUIRE(*my_iter == 'b');
        }

        SECTION("dereference postincrement")
        {
            auto old_val = *my_iter++;

            REQUIRE(old_val == 'a');
            REQUIRE(*my_iter == 'b');
        }
    }

    SECTION("another mock vector and another iterator")
    {
        std::vector<char> mock_nodes2{'x', 'y', 'z'};

        helene::persistent_iterator_<char, int> my_iter2(1, mock_nodes2);

        REQUIRE(my_iter != my_iter2);

        SECTION("swap the two iterators")
        {
            std::swap(my_iter, my_iter2);

            REQUIRE(*my_iter2 == 'a');
            REQUIRE(*my_iter == 'y');
            REQUIRE(my_iter != my_iter2);
        }

        SECTION("predecrement second iterator")
        {
            --my_iter2;

            REQUIRE(*my_iter2 == 'x');

            SECTION("increment iterator with 2")
            {
                my_iter2 += 2;

                REQUIRE(*my_iter2 == 'z');
            }

            SECTION("add 2 to iterator")
            {
                auto my_iter3 = my_iter2 + 2;

                REQUIRE(*my_iter3 == 'z');
                REQUIRE(my_iter2 < my_iter3);
                REQUIRE(my_iter2 <= my_iter3);
                REQUIRE(my_iter3 > my_iter2);
                REQUIRE(my_iter3 >= my_iter2);
            }

            SECTION("add iterator to 2")
            {
                auto my_iter3 = 2 + my_iter2;

                REQUIRE(*my_iter3 == 'z');
            }
        }

        SECTION("postdecrement second iterator")
        {
            my_iter2--;

            REQUIRE(*my_iter2 == 'x');
        }

        SECTION("subtract 1 from iterator")
        {
            auto my_iter3 = my_iter2 - 1;

            REQUIRE(*my_iter3 == 'x');
        }
    }

    SECTION("assign to dereferenced iterator")
    {
        *my_iter = 'c';

        REQUIRE(*my_iter == 'c');
        REQUIRE(mock_nodes[0] == 'c');
    }

    SECTION("postincrement assignment")
    {
        *my_iter++ = 'c';

        REQUIRE(mock_nodes[0] == 'c');
    }
}

TEST_CASE("persistent_iterator_ to mock vector of a mock struct",
          "[persistent_iterator_]")
{
    struct has_function
    {
        int i;

        int
        get_i() const
        {
            return i;
        }
    };

    std::vector<has_function> mock_nodes{{0}, {1}, {2}, {4}, {8}};

    helene::persistent_iterator_<has_function, int> my_iter(2, mock_nodes);

    REQUIRE(my_iter->get_i() == 2);
}

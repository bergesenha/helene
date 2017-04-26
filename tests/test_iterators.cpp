#include "catch.hpp"


#include <dag.hpp>
#include <utility>


TEST_CASE("persistent_iterator_ to mock vector", "[presistent_iterator_]")
{

    std::vector<char> mock_nodes;
    mock_nodes.push_back('a');

    helene::persistent_iterator_<char, int> my_iter(0, mock_nodes);

    REQUIRE(*my_iter == 'a');


    SECTION("copy construct iterator")
    {
        helene::persistent_iterator_<char, int> my_iter2 = my_iter;

        REQUIRE(*my_iter2 == 'a');
        REQUIRE(my_iter == my_iter2);
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

        SECTION("preincrement iterator")
        {
            ++my_iter;

            REQUIRE(*my_iter == 'b');
        }

        SECTION("posincrement iterator")
        {
            auto old = my_iter++;

            REQUIRE(*old == 'a');
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

#include "catch.hpp"


#include <iterators.hpp>
#include <utility>
#include <algorithm>


TEST_CASE("persistent_iterator_ to mock vector", "[presistent_iterator_]")
{

    std::vector<char> mock_nodes;
    mock_nodes.push_back('a');

    helene::persistent_iterator_<char> my_iter(0, mock_nodes);

    REQUIRE(*my_iter == 'a');
    REQUIRE(my_iter[0] == 'a');


    SECTION("copy construct iterator")
    {
        helene::persistent_iterator_<char> my_iter2 = my_iter;

        REQUIRE(*my_iter2 == 'a');
        REQUIRE(my_iter == my_iter2);

        REQUIRE(my_iter <= my_iter2);
        REQUIRE(my_iter >= my_iter2);
    }

    SECTION("move construct iterator")
    {
        helene::persistent_iterator_<char> my_iter2(std::move(my_iter));

        REQUIRE(*my_iter2 == 'a');
    }

    SECTION("copy assign iterator")
    {
        helene::persistent_iterator_<char> my_iter2(10, mock_nodes);

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

        helene::persistent_iterator_<char> my_iter2(1, mock_nodes2);

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

    helene::persistent_iterator_<has_function> my_iter(2, mock_nodes);

    REQUIRE(my_iter->get_i() == 2);
}


TEST_CASE("ordered_iterator_ with mock order and mock vector",
          "[ordered_iterator_")
{
    std::vector<char> mock_vec{'c', 'e', 'b', 'a', 'd'};
    std::vector<std::vector<char>::size_type> mock_order{3, 2, 0, 4, 1};

    helene::ordered_iterator_<char> it1(0, mock_order, mock_vec);

    REQUIRE(*it1 == 'a');

    REQUIRE(it1[0] == 'a');
    REQUIRE(it1[1] == 'b');
    REQUIRE(it1[4] == 'e');


    SECTION("preincrement iterator")
    {
        ++it1;

        REQUIRE(*it1 == 'b');

        REQUIRE(it1[-1] == 'a');
        REQUIRE(it1[1] == 'c');


        SECTION("predecrement iterator")
        {
            auto it2 = --it1;

            REQUIRE(*it2 == 'a');
            REQUIRE(*it1 == 'a');

            REQUIRE(it2 == it1);
            REQUIRE(it2 >= it1);
            REQUIRE(it2 <= it1);
        }

        SECTION("postdecrement iterator")
        {
            auto it2 = it1--;

            REQUIRE(*it2 == 'b');
            REQUIRE(*it1 == 'a');

            REQUIRE(it2 > it1);
            REQUIRE(it2 >= it1);
            REQUIRE(it1 < it2);
            REQUIRE(it1 <= it2);
        }
    }

    SECTION("postincrement iterator")
    {
        auto it2 = it1++;

        REQUIRE(*it2 == 'a');
        REQUIRE(*it1 == 'b');
    }

    SECTION("copy iterator")
    {
        auto it2 = it1;

        REQUIRE(*it2 == 'a');
        REQUIRE(it2 == it1);

        SECTION("preincrement second iterator")
        {
            ++it2;

            REQUIRE(*it2 == 'b');
            REQUIRE(it2 != it1);
        }
    }

    SECTION("add integer to iterator")
    {
        auto it2 = it1 + 3;

        REQUIRE(*it2 == 'd');

        SECTION("subtract integer from iterator")
        {
            auto it3 = it2 - 3;

            REQUIRE(*it3 == 'a');
            REQUIRE(it2 - it3 == 3);
            REQUIRE(it3 - it2 == -3);
        }

        SECTION("convert to persistent_iterator_")
        {
            helene::persistent_iterator_<char> converted = it2;

            REQUIRE(*converted == *it2);
        }
    }

    SECTION("add iterator to integer")
    {
        auto it2 = 3 + it1;

        REQUIRE(*it2 == 'd');
    }

    SECTION("append integer to iterator")
    {
        it1 += 3;

        REQUIRE(*it1 == 'd');

        SECTION("subpend integer from iterator")
        {
            it1 -= 3;

            REQUIRE(*it1 == 'a');
        }
    }

    SECTION("construct persistent_iterator_ from ordered_iterator_")
    {
        helene::persistent_iterator_<char> converted(it1);

        REQUIRE(*it1 == *converted);
    }
}

TEST_CASE("ordered iterator with mock vector of mock structs",
          "[ordere_iterator_]")
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

    std::vector<has_function> mock_nodes{{2}, {4}, {1}, {0}, {3}};
    std::vector<std::vector<has_function>::size_type> mock_order{3, 2, 0, 4, 1};

    helene::ordered_iterator_<has_function> it1(0, mock_order, mock_nodes);


    REQUIRE(it1->get_i() == 0);
}


TEST_CASE("test std algorithms on ordered_iterator_", "[ordered_iterator_]")
{
    std::vector<char> mock_vec{'a', 'b', 'c', 'd', 'e'};
    std::vector<std::vector<char>::size_type> mock_order{2, 1, 0, 3, 4};

    helene::ordered_iterator_<char> begin_unordered(0, mock_order, mock_vec);
    helene::ordered_iterator_<char> end_unordered(
        mock_order.size(), mock_order, mock_vec);

    REQUIRE(*begin_unordered != 'a');

    SECTION("sort on ordered_iterator_")
    {
        std::sort(begin_unordered, end_unordered);

        REQUIRE(mock_vec[0] == 'c');
        REQUIRE(mock_vec[1] == 'b');
        REQUIRE(mock_vec[2] == 'a');
        REQUIRE(mock_vec[3] == 'd');
        REQUIRE(mock_vec[4] == 'e');
    }
}

#include "catch.hpp"


#include <parallel.hpp>
#include <vector>
#include <deque>


TEST_CASE("test parallel_remove with parallel vectors", "[parallel_remove]")
{

    std::vector<int> intrange{1, 2, 3, 4};
    std::vector<char> charange{'a', 'b', 'c', 'd'};

    SECTION("remove by value")
    {
        auto rem_it = helene::parallel_remove(
            intrange.begin(), intrange.end(), charange.begin(), 3);

        intrange.erase(rem_it.first, intrange.end());
        charange.erase(rem_it.second, charange.end());

        REQUIRE(intrange.size() == 3);
        REQUIRE(charange.size() == 3);
        REQUIRE(intrange[0] == 1);
        REQUIRE(charange[0] == 'a');
        REQUIRE(intrange[1] == 2);
        REQUIRE(charange[1] == 'b');
        REQUIRE(intrange[2] == 4);
        REQUIRE(charange[2] == 'd');
    }
}

TEST_CASE("test parallel_remove with one vector and one deque",
          "[parallel_remove]")
{
    std::vector<int> intrange{1, 2, 3, 4, 5, 6};
    std::deque<std::string> stringrange{
        "one", "two", "three", "four", "five", "six"};

    auto rem_pair = helene::parallel_remove(
        intrange.begin(), intrange.end(), stringrange.begin(), 2);


    intrange.erase(rem_pair.first, intrange.end());
    stringrange.erase(rem_pair.second, stringrange.end());


    REQUIRE(intrange.size() == 5);
    REQUIRE(stringrange.size() == 5);
    REQUIRE(intrange[1] == 3);
    REQUIRE(stringrange[1] == std::string("three"));
}

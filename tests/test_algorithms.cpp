#include "catch.hpp"


#include <parallel.hpp>


TEST_CASE("test parallel_remove", "[parallel_remove]")
{

    std::vector<int> intrange{1, 2, 3, 4};
    std::vector<char> charange{'a', 'b', 'c', 'd'};

    auto rem_it = helene::parallel_remove(
        intrange.begin(), intrange.end(), charange.begin(), 3);

    intrange.erase(rem_it.first, intrange.end());
    charange.erase(rem_it.second, charange.end());

    REQUIRE(intrange.size() == 3);
    REQUIRE(charange.size() == 3);
}

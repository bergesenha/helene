#include "catch.hpp"

#include <array_view.hpp>


TEST_CASE("create array_view from array of ints", "[array_view]")
{
    int my_array[] = {1, 2, 3, 4, 5, 6};

    helene::array_view<int> av(my_array);

    REQUIRE(av.size() == 6);
    REQUIRE(av.empty() == false);
    REQUIRE(av[5] == 6);

    SECTION("get iterators to elements")
    {
        auto f = av.begin();
        auto l = av.end();

        REQUIRE(*f == 1);
    }
}


TEST_CASE("create array_view from const array of double", "[make_array_view]")
{
    const double my_array[] = {1.1, 2.2, 3.3};

    auto av = helene::make_array_view(my_array);

    auto f = av.begin();
}

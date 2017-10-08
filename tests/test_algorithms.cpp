#include "catch.hpp"

#include <algorithms.hpp>


TEST_CASE("test parallel_for_each", "[parallel_for_each]")
{
    struct has_value
    {
        char metadata;
        int value;
    };

    std::vector<has_value> values1{{'a', 1}, {'b', 2}, {'c', 3}};
    std::vector<int> values2{4, 5, 6};

    SECTION("mutate value field of has_values in values1 with values2")
    {
        helene::parallel_for_each(values1.begin(),
                                  values1.end(),
                                  values2.begin(),
                                  [](has_value& hv, int v) { hv.value = v; });

        REQUIRE(values1[0].value == 4);
        REQUIRE(values1[1].value == 5);
        REQUIRE(values1[2].value == 6);
    }
}

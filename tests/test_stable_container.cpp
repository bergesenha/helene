#include <catch.hpp>

#include <stable_container.hpp>


TEST_CASE("", "")
{
    helene::stable_vector<int> sv;
    sv.add(10);

    CHECK(sv[0] == 10);
}

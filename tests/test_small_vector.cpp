#include <catch.hpp>
#include <small_vector.hpp>


TEST_CASE("default construct", "[small_vector]")
{
    helene::small_vector<int> intvec;

    CHECK(intvec.size() == 0);
    CHECK(intvec.on_stack());

    SECTION("push back a value")
    {
        intvec.push_back(1);

        CHECK(intvec[0] == 1);
        CHECK(intvec.size() == 1);
        CHECK(intvec.on_stack());
    }

    SECTION("push back 2 values")
    {
        intvec.push_back(1);
        intvec.push_back(2);

        CHECK(intvec[0] == 1);
        CHECK(intvec[1] == 2);
        CHECK(intvec.size() == 2);
        CHECK(intvec.on_stack());
    }

    SECTION("push back enough values to fill stack size")
    {
        for(std::size_t i = 0; i < intvec.max_stack_size(); ++i)
        {
            intvec.push_back(i * 10);
        }

        CHECK(intvec.size() == intvec.max_stack_size());
        CHECK(intvec.on_stack());


        SECTION("push back another element")
        {
            intvec.push_back(100);

            CHECK(intvec.size() == intvec.max_stack_size() + 1);
            CHECK(intvec[intvec.max_stack_size()] == 100);
            CHECK(!intvec.on_stack());
        }
    }
}

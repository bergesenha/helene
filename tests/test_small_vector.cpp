#include <catch.hpp>
#include <small_vector.hpp>


TEST_CASE("default construct", "[small_vector]")
{
    helene::small_vector<int> intvec;

    CHECK(intvec.size() == 0);
    CHECK(intvec.on_stack());
    CHECK(std::distance(intvec.begin(), intvec.end()) == 0);

    SECTION("push back a value")
    {
        intvec.push_back(1);

        CHECK(intvec[0] == 1);
        CHECK(intvec.size() == 1);
        CHECK(std::distance(intvec.begin(), intvec.end()) == 1);
        CHECK(intvec.on_stack());

        SECTION("pop back the value")
        {
            intvec.pop_back();

            CHECK(intvec.size() == 0);
            CHECK(intvec.empty());
            CHECK(intvec.on_stack());
        }
    }

    SECTION("push back 2 values")
    {
        intvec.push_back(1);
        intvec.push_back(2);

        CHECK(intvec[0] == 1);
        CHECK(intvec[1] == 2);
        CHECK(intvec.size() == 2);
        CHECK(std::distance(intvec.begin(), intvec.end()) == 2);
        CHECK(intvec.on_stack());

        SECTION("copy values via iterators")
        {
            std::vector<int> check_vec(intvec.begin(), intvec.end());

            CHECK(check_vec[0] == 1);
            CHECK(check_vec[1] == 2);
        }

        SECTION("pop back")
        {
            intvec.pop_back();

            CHECK(intvec[0] == 1);
            CHECK(intvec.size() == 1);
            CHECK(intvec.on_stack());
        }
    }

    SECTION("push back enough values to fill stack size")
    {
        for(std::size_t i = 0; i < intvec.max_stack_size(); ++i)
        {
            intvec.push_back(i * 10);
        }

        CHECK(intvec.size() == intvec.max_stack_size());
        CHECK(std::distance(intvec.begin(), intvec.end()) ==
              intvec.max_stack_size());
        CHECK(intvec.on_stack());

        SECTION("copy values via iterators")
        {
            std::vector<int> check_vec(intvec.begin(), intvec.end());

            for(std::size_t i = 0; i < intvec.size(); ++i)
            {
                CHECK(check_vec[i] == intvec[i]);
            }
        }

        SECTION("pop back an element")
        {
            intvec.pop_back();

            for(std::size_t i = 0; i < intvec.size(); ++i)
            {
                CHECK(intvec[i] == i * 10);
            }
            CHECK(intvec.size() == intvec.max_stack_size() - 1);
            CHECK(intvec.on_stack());
        }

        SECTION("push back another element")
        {
            intvec.push_back(100);

            CHECK(intvec.size() == intvec.max_stack_size() + 1);
            CHECK(std::distance(intvec.begin(), intvec.end()) ==
                  intvec.max_stack_size() + 1);
            CHECK(intvec[intvec.max_stack_size()] == 100);
            CHECK(!intvec.on_stack());

            SECTION("copy values via iterators")
            {
                std::vector<int> check_vec(intvec.begin(), intvec.end());

                for(std::size_t i = 0; i < intvec.size(); ++i)
                {
                    CHECK(check_vec[i] == intvec[i]);
                }
            }

            SECTION("pop back an element")
            {
                intvec.pop_back();

                CHECK(intvec.on_stack());
            }
        }
    }
}

TEST_CASE("construct with initializer_list of size below max stack size",
          "[small_vector]")
{
    helene::small_vector<int, 10 * sizeof(int)> intvec{10, 20, 30};

    CHECK(intvec.size() == 3);
    CHECK(intvec[0] == 10);
    CHECK(intvec[1] == 20);
    CHECK(intvec[2] == 30);
    CHECK(intvec.on_stack());
}


TEST_CASE("construct with initalizer_list of size equalling max stack size",
          "[small_vector]")
{
    helene::small_vector<int, 10 * sizeof(int)> intvec{
        10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    CHECK(intvec.size() == 10);
    CHECK(intvec.on_stack());

    for(int i = 0; i < 10; ++i)
    {
        CHECK(intvec[i] == 10 * (i + 1));
    }

    SECTION("push_back element to exceed stack size")
    {
        intvec.push_back(110);

        CHECK(!intvec.on_stack());

        for(int i = 0; i < 10; ++i)
        {
            CHECK(intvec[i] == 10 * (i + 1));
        }
        CHECK(intvec[10] == 110);
    }
}


TEST_CASE("construct with initializer_list of size above max stack size",
          "[small_vector]")
{
    helene::small_vector<int> intvec{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    CHECK(intvec.size() == 10);
    CHECK(!intvec.on_stack());

    for(int i = 0; i < 10; ++i)
    {
        CHECK(intvec[i] == 10 * (i + 1));
    }
}

TEST_CASE("construct a const small_vector", "[small_vector]")
{
    const helene::small_vector<int> intvec{10, 20, 30};

    SECTION("copy elements via iterators")
    {
        std::vector<int> check_vec(intvec.cbegin(), intvec.cend());

        for(auto i = 0ul; i < intvec.size(); ++i)
        {
            CHECK(intvec[i] == check_vec[i]);
        }
    }
}

TEST_CASE("construct a const small_vector above stack size", "[small_vector]")
{
    const helene::small_vector<int> intvec{
        10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    SECTION("copy elements via iterators")
    {
        std::vector<int> check_vec(intvec.cbegin(), intvec.cend());

        for(auto i = 0ul; i < intvec.size(); ++i)
        {
            CHECK(intvec[i] == check_vec[i]);
        }
    }
}

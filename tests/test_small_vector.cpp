#include <catch.hpp>
#include <small_vector.hpp>


TEST_CASE("default construct", "[small_vector]")
{
    helene::small_vector<int> intvec;

    CHECK(intvec.size() == 0);
    CHECK(intvec.on_stack());
    CHECK(std::distance(intvec.begin(), intvec.end()) == 0);


    SECTION("copy construct")
    {
        helene::small_vector<int> intvec_cp = intvec;

        CHECK(intvec_cp.size() == 0);
        CHECK(intvec_cp.on_stack());

        SECTION("modify original")
        {
            intvec.push_back(55);

            CHECK(intvec_cp.size() == 0);
            CHECK(intvec_cp.on_stack());
        }
    }

    SECTION("copy assign")
    {
        helene::small_vector<int> intvec2;

        intvec2 = intvec;

        CHECK(intvec2.size() == 0);
        CHECK(intvec2.on_stack());
        CHECK(std::distance(intvec2.begin(), intvec2.end()) == 0);
    }

    SECTION("push back a value")
    {
        intvec.push_back(1);

        CHECK(intvec[0] == 1);
        CHECK(intvec.size() == 1);
        CHECK(std::distance(intvec.begin(), intvec.end()) == 1);
        CHECK(intvec.on_stack());
        CHECK(intvec.front() == 1);
        CHECK(intvec.back() == 1);

        SECTION("copy construct")
        {
            helene::small_vector<int> intvec_cp = intvec;

            CHECK(intvec[0] == 1);
            CHECK(intvec.size() == 1);
            CHECK(std::distance(intvec.begin(), intvec.end()) == 1);
            CHECK(intvec.on_stack());
            CHECK(intvec.front() == 1);
            CHECK(intvec.back() == 1);

            CHECK(intvec_cp[0] == 1);
            CHECK(intvec_cp.size() == 1);
            CHECK(std::distance(intvec_cp.begin(), intvec_cp.end()) == 1);
            CHECK(intvec_cp.on_stack());
            CHECK(intvec_cp.front() == 1);
            CHECK(intvec_cp.back() == 1);
        }

        SECTION("copy assign")
        {
            helene::small_vector<int> intvec2;
            intvec2 = intvec;

            CHECK(intvec2[0] == 1);
            CHECK(intvec2.size() == 1);
            CHECK(std::distance(intvec2.begin(), intvec2.end()) == 1);
            CHECK(intvec2.on_stack());
            CHECK(intvec2.front() == 1);
            CHECK(intvec2.back() == 1);
        }

        SECTION("pop back the value")
        {
            intvec.pop_back();

            CHECK(intvec.size() == 0);
            CHECK(intvec.empty());
            CHECK(intvec.on_stack());
        }

        SECTION("erase the value")
        {
            auto res = intvec.erase(intvec.begin());

            CHECK(intvec.size() == 0);
            CHECK(intvec.on_stack());
            CHECK(res == intvec.end());
        }

        SECTION("range erase the value")
        {
            auto res = intvec.erase(intvec.begin(), intvec.end());

            CHECK(intvec.size() == 0);
            CHECK(intvec.on_stack());
            CHECK(res == intvec.end());
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

        SECTION("erase first element")
        {
            auto res = intvec.erase(intvec.begin());

            CHECK(intvec.size() == 1);
            CHECK(intvec[0] == 2);
            CHECK(*res == 2);
            CHECK(intvec.on_stack());
        }

        SECTION("erase last element")
        {
            auto res = intvec.erase(intvec.begin() + 1);
            CHECK(res == intvec.end());
            CHECK(intvec[0] == 1);
            CHECK(intvec.size() == 1);
        }

        SECTION("range erase both elements")
        {
            auto res = intvec.erase(intvec.begin(), intvec.end());
            CHECK(res == intvec.end());
            CHECK(intvec.empty());
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
            CHECK(intvec.front() == 0);
        }

        SECTION("push back another element")
        {
            intvec.push_back(100);

            CHECK(intvec.size() == intvec.max_stack_size() + 1);
            CHECK(std::distance(intvec.begin(), intvec.end()) ==
                  intvec.max_stack_size() + 1);
            CHECK(intvec[intvec.max_stack_size()] == 100);
            CHECK(!intvec.on_stack());
            CHECK(intvec.back() == 100);

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

                for(std::size_t i = 0; i < intvec.size(); ++i)
                {
                    CHECK(intvec[i] == i * 10);
                }
            }

            SECTION("erase 5th element")
            {
                auto res = intvec.erase(intvec.begin() + 4);

                CHECK(*res == 50);
                CHECK(intvec.size() == intvec.max_stack_size());
                CHECK(intvec.back() == 100);
                CHECK(intvec.front() == 0);
                CHECK(intvec.on_stack());
            }

            SECTION("erase last element")
            {
                auto res = intvec.erase(intvec.end() - 1);

                CHECK(res == intvec.end());
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

    SECTION("erase two first elements")
    {
        auto res = intvec.erase(intvec.begin(), intvec.begin() + 2);

        CHECK(*res == 30);
        CHECK(intvec.size() == 1);
        CHECK(intvec.on_stack());
    }

    SECTION("erase middle element")
    {
        auto res = intvec.erase(intvec.begin() + 1, intvec.begin() + 2);

        CHECK(*res == 30);
        CHECK(intvec[0] == 10);
        CHECK(intvec.size() == 2);
        CHECK(intvec.on_stack());
    }
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
    helene::small_vector<int, 6 * sizeof(int)> intvec{
        10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    CHECK(intvec.size() == 10);
    CHECK(!intvec.on_stack());

    for(int i = 0; i < 10; ++i)
    {
        CHECK(intvec[i] == 10 * (i + 1));
    }

    SECTION("copy construct")
    {
        helene::small_vector<int, 6 * sizeof(int)> intvec_cp = intvec;

        CHECK(intvec_cp.size() == 10);
        CHECK(!intvec_cp.on_stack());

        for(int i = 0; i < 10; ++i)
        {
            CHECK(intvec_cp[i] == 10 * (i + 1));
        }

        SECTION("modify original")
        {
            intvec.erase(intvec.begin(), intvec.end() - 1);

            CHECK(intvec_cp.size() == 10);
            CHECK(!intvec_cp.on_stack());

            for(int i = 0; i < 10; ++i)
            {
                CHECK(intvec_cp[i] == 10 * (i + 1));
            }
        }
    }

    SECTION("copy assign")
    {
        helene::small_vector<int, 6 * sizeof(int)> intvec2;

        intvec2 = intvec;

        CHECK(intvec2.size() == 10);
        CHECK(!intvec2.on_stack());

        for(int i = 0; i < 10; ++i)
        {
            CHECK(intvec2[i] == 10 * (i + 1));
        }
    }

    SECTION("pop back an element")
    {
        intvec.pop_back();

        CHECK(!intvec.on_stack());
    }

    SECTION("erase 5th element")
    {
        auto res = intvec.erase(intvec.begin() + 4);

        CHECK(intvec.size() == 9);
        CHECK(*res == 60);
        CHECK(!intvec.on_stack());
    }

    SECTION("erase first three elements")
    {
        auto res = intvec.erase(intvec.begin(), intvec.begin() + 3);

        CHECK(*res == 40);
        CHECK(intvec.size() == 7);
        CHECK(!intvec.on_stack());
    }

    SECTION("erase last three elements")
    {
        auto res = intvec.erase(intvec.end() - 3, intvec.end());

        CHECK(res == intvec.end());
        CHECK(intvec.size() == 7);
        CHECK(intvec[6] == 70);
        CHECK(!intvec.on_stack());
    }

    SECTION("erase from 3rd to 8th element")
    {
        auto res = intvec.erase(intvec.begin() + 2, intvec.begin() + 8);

        CHECK(*res == 90);
        CHECK(intvec.size() == 4);
        CHECK(intvec.on_stack());
        CHECK(intvec[0] == 10);
        CHECK(intvec[1] == 20);
        CHECK(intvec[2] == 90);
        CHECK(intvec[3] == 100);
    }

    SECTION("erase from 3rd element to last")
    {
        auto res = intvec.erase(intvec.begin() + 2, intvec.end());

        CHECK(res == intvec.end());
        CHECK(intvec.size() == 2);
        CHECK(intvec.on_stack());
        CHECK(intvec[0] == 10);
        CHECK(intvec[1] == 20);
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

    CHECK(intvec.front() == 10);
    CHECK(intvec.back() == 100);

    SECTION("copy elements via iterators")
    {
        std::vector<int> check_vec(intvec.cbegin(), intvec.cend());

        for(auto i = 0ul; i < intvec.size(); ++i)
        {
            CHECK(intvec[i] == check_vec[i]);
        }
    }
}

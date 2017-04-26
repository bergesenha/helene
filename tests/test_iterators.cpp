#include "catch.hpp"


#include <dag.hpp>


TEST_CASE("persistent_iterator_ to mock vector", "[presistent_iterator_]")
{

    std::vector<char> mock_nodes;
    mock_nodes.push_back('a');

    helene::persistent_iterator_<char, int> my_iter(0, mock_nodes);

    REQUIRE(*my_iter == 'a');

    SECTION("add another element in mock vector")
    {
        mock_nodes.push_back('b');

        REQUIRE(*my_iter == 'a');

        SECTION("preincrement iterator")
        {
            ++my_iter;

            REQUIRE(*my_iter == 'b');
        }
    }
}

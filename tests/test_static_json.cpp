#include <catch.hpp>

#include <static_json.hpp>


struct data_name
{
    static constexpr const char* value = "data";
};


using helene::static_json::field;
using helene::static_json::json;
using helene::static_json::value_type;


TEST_CASE("", "")
{
    json<field<data_name, int>> js1;

    js1.get<data_name>() = 23;

    CHECK(js1.get<data_name>() == 23);
}

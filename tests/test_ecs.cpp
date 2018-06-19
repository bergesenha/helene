#include <catch.hpp>

#include <ecs.hpp>


enum class TableName
{
    Numbers,
    Names
};


TEST_CASE("", "")
{
    helene::database<
        TableName,
        helene::table_description<TableName, TableName::Numbers, int>,
        helene::table_description<TableName, TableName::Names, std::string>>
        db;
}

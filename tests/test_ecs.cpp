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

    const auto number_index_0 = db.insert<TableName::Numbers>(10);
    const auto name_index_0 =
        db.insert<TableName::Names>(std::string("harald"));
}

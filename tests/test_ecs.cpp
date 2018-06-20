#include <catch.hpp>

#include <ecs.hpp>


enum class TableName
{
    Numbers,
    Names
};


TEST_CASE("", "")
{
    helene::table<
        TableName,
        helene::column_description<TableName, TableName::Numbers, int>,
        helene::column_description<TableName, TableName::Names, std::string>>
        db;

    const auto number_index_0 = db.insert<TableName::Numbers>(10);
    const auto name_index_0 =
        db.insert<TableName::Names>(std::string("harald"));

    CHECK(db.get<TableName::Numbers>(number_index_0) == 10);
    CHECK(db.get<TableName::Names>(name_index_0) == std::string("harald"));
}

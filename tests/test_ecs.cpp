#include <catch.hpp>

#include <ecs.hpp>


enum class ColumnNames
{
    Surname,
    Age,
    Height
};

template <ColumnNames Name, class T>
using mycolumn = helene::column_description<ColumnNames, Name, T>;


TEST_CASE("", "")
{
    helene::table<ColumnNames,
                  mycolumn<ColumnNames::Surname, std::string>,
                  mycolumn<ColumnNames::Age, int>,
                  mycolumn<ColumnNames::Height, float>>
        tb;

    const auto first_row = tb.insert_row("Bergesen", 35, 1.83f);
    const auto second_row = tb.insert_row("Lund-Hansen", 70, 1.80f);

    CHECK(tb.get<ColumnNames::Surname>(first_row) == "Bergesen");
    CHECK(tb.get<ColumnNames::Surname>(second_row) == "Lund-Hansen");
    CHECK(tb.get<ColumnNames::Age>(first_row) == 35);
    CHECK(tb.get<ColumnNames::Age>(second_row) == 70);

    CHECK(tb.size() == 2);

    SECTION("erase first row")
    {
        tb.erase_row(first_row);

        CHECK(tb.get<ColumnNames::Surname>(second_row) == "Lund-Hansen");
        CHECK(tb.get<ColumnNames::Age>(second_row) == 70);

        CHECK(tb.size() == 1);
    }

    SECTION("erase second row")
    {
        tb.erase_row(second_row);

        CHECK(tb.get<ColumnNames::Surname>(first_row) == "Bergesen");
        CHECK(tb.get<ColumnNames::Age>(first_row) == 35);

        CHECK(tb.size() == 1);
    }
}

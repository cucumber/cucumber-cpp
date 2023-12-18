#include <gtest/gtest.h>

#include <cucumber-cpp/internal/Table.hpp>
using namespace cucumber::internal;

TEST(TableTest, forbidsRowsNotMatchingTableColumnsSize) {
    Table t;
    t.addColumn("C1");

    EXPECT_THROW(
        {
            Table::row_type row;
            t.addRow(row);
        },
        std::range_error
    );

    EXPECT_NO_THROW({ t.addRow({"R1"}); });

    EXPECT_THROW({ t.addRow({"R1", "R2"}); }, std::range_error);
}

TEST(TableTest, rowsCannotBeAddedBeforeColumnsAreSet) {
    Table t;

    EXPECT_THROW(
        {
            Table::row_type row;
            t.addRow(row);
        },
        std::runtime_error
    );
}

TEST(TableTest, columnsCannotBeChangesAfterRowsAreAdded) {
    Table t;
    t.addColumn("C1");
    t.addRow({"R1"});

    EXPECT_THROW({ t.addColumn("C2"); }, std::runtime_error);
}

TEST(TableTest, addedRowsMatchColumnDefinition) {
    Table t;
    t.addColumn("C1");
    t.addColumn("C2");
    t.addColumn("C3");

    ASSERT_EQ(0, t.hashes().size());

    t.addRow({"R11", "R12", "R13"});
    Table::hashes_type hashes = t.hashes();
    ASSERT_EQ(1, hashes.size());
    EXPECT_EQ("R11", hashes[0]["C1"]);
    EXPECT_EQ("R12", hashes[0]["C2"]);
    EXPECT_EQ("R13", hashes[0]["C3"]);

    t.addRow({"R21", "R22", "R23"});
    t.addRow({"R31", "R32", "R33"});
    t.addRow({"R41", "R42", "R43"});
    hashes = t.hashes();
    ASSERT_EQ(4, hashes.size());
    EXPECT_EQ("R21", hashes[1]["C1"]);
    EXPECT_EQ("R32", hashes[2]["C2"]);
    EXPECT_EQ("R43", hashes[3]["C3"]);
}

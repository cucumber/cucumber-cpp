#include <QTest>
// Pretend to be GTest
#define EXPECT_EQ QCOMPARE
#define ASSERT_TRUE QVERIFY
#include "CalculatorQtSteps.cpp"

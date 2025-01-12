#include <QtTest>
#include "../../../src/Models/CLinkerScriptFile.h"
#include "../../../src/DrcEngine/Rules/CEntryIsDefinedRule.h"

using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::DrcEngine::Rules;


class TestCEntryIsDefinedRule : public QObject
{
    Q_OBJECT

private slots:
    void testEntryDefined();
    void testEntryNotDefined();
};

void TestCEntryIsDefinedRule::testEntryDefined()
{
    // Arrange
    auto linkerScriptFile = std::make_shared<CLinkerScriptFile>();
    // Add data for a valid entry point

    CEntryIsDefinedRule rule;

    // Act
    auto violations = rule.PerformCheck(linkerScriptFile);

    // Assert
    QCOMPARE(violations.size(), 0); // Expect no violations
}

void TestCEntryIsDefinedRule::testEntryNotDefined()
{
    // Arrange
    auto linkerScriptFile = std::make_shared<VisualLinkerScript::Models::CLinkerScriptFile>();
    // Add data with a missing entry point

    CEntryIsDefinedRule rule;

    // Act
    auto violations = rule.PerformCheck(linkerScriptFile);

    // Assert
    QVERIFY(!violations.empty()); // Expect at least one violation
}

QTEST_MAIN(TestCEntryIsDefinedRule)
#include "TestCEntryIsDefinedRule.moc"
#include <QtTest/QtTest>
#include <QObject>

#include "TestMacros.h"
#include "Models/CLinkerScriptFile.h"
#include "DrcEngine/Rules/CEntryIsDefinedRule.h"

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
    // auto linkerScriptFile = std::make_shared<CLinkerScriptFile>();
    auto linkerScriptFile = std::make_shared<CLinkerScriptFile>("","","", false, "");
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
    auto linkerScriptFile = std::make_shared<CLinkerScriptFile>("", "", "", false, "");
    // Add data with a missing entry point

    CEntryIsDefinedRule rule;

    // Act
    auto violations = rule.PerformCheck(linkerScriptFile);

    // Assert
    QVERIFY(!violations.empty()); // Expect at least one violation
}

#include "TestCEntryIsDefinedRule.moc"
REGISTER_TEST(TestCEntryIsDefinedRule)

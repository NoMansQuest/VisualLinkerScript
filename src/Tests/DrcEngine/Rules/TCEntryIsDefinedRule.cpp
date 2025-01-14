#include <QtTest/QtTest>
#include <QObject>

#include "../../TestMacros.h"
#include "../../Models/CLinkerScriptFile.h"
#include "../../DrcEngine/Rules/CEntryIsDefinedRule.h"
#include "RulesTestsHelper.h"
#include "Models/CFunctionCall.h"

using namespace VisualLinkerScript::Models;
using namespace VisualLinkerScript::DrcEngine::Rules;
using namespace VisualLinkerScript::Tests::DrcEngine::Rules;

class TCEntryIsDefinedRule : public QObject
{
    Q_OBJECT

private slots:
    void testEntryDefined();
    void testEntryNotDefined();
    void testEntryDefinedMultipleTimes();
};


void TCEntryIsDefinedRule::testEntryDefined()
{
    // Arrange
    auto linkerScriptFile = std::make_shared<SuperCLinkerScriptFile>();

    CEntryIsDefinedRule rule;

    // Act
    auto violations = rule.PerformCheck(linkerScriptFile);

    // Assert
    QVERIFY(violations.empty());
}

void TCEntryIsDefinedRule::testEntryNotDefined()
{
    // Arrange
    auto linkerScriptFile = std::make_shared<SuperCLinkerScriptFile>();
    CEntryIsDefinedRule rule;

    // Act
    auto violations = rule.PerformCheck(linkerScriptFile);

    // Assert
    QVERIFY(violations.empty());
}

void TCEntryIsDefinedRule::testEntryDefinedMultipleTimes()
{
    // Arrange
    auto contentSource = std::make_shared<SuperCLinkerScriptFile>();
    IntroduceFunction(contentSource, "ENTRY", "Something");
    IntroduceFunction(contentSource, "ENTRY", "SomethingElse");

    CEntryIsDefinedRule rule;

    // Act
    auto violations = rule.PerformCheck(contentSource);

    // Assert
    QVERIFY(violations.empty());
}

#include "TCEntryIsDefinedRule.moc"
REGISTER_TEST(TCEntryIsDefinedRule)
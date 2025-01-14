#pragma once
#include "SuperCLinkerScriptFile.h"
#include "Models/CExpression.h"
#include "Models/CFunctionCall.h"
#include "Models/CSymbol.h"

namespace VisualLinkerScript::Tests::DrcEngine::Rules
{
	/// @brief Introduce a normal function containing string input.
	inline void IntroduceFunction(
		std::shared_ptr<CLinkerScriptFile> targetLinkerScript,
		const std::string& functionName,
		const std::string& functionContent)
	{

		auto functionNameEntry = CRawEntry(functionName, RawEntryType::Word);
		auto parenthesisOpenEntry = CRawEntry(functionName, RawEntryType::ParenthesisOpen);
		auto parenthesisCloseEntry = CRawEntry(functionName, RawEntryType::ParenthesisClose);
		auto innerContentContentSymbol = CRawEntry(functionContent, RawEntryType::String);

		auto innerContentContent = std::shared_ptr<CParsedContentBase>(
			new CSymbol(
				innerContentContentSymbol,
				{ innerContentContentSymbol },
				{}));

		auto innerContentExpression = std::shared_ptr<CParsedContentBase>(
			new CExpression(
				CRawEntry(),
				CRawEntry(),
				{ innerContentContent },
				{ innerContentContentSymbol },
				{}));

		auto functionCall = std::shared_ptr<CParsedContentBase>(
			new CFunctionCall(
				functionNameEntry,
				parenthesisOpenEntry,
				parenthesisCloseEntry,
				{ innerContentExpression },
				{ functionNameEntry, parenthesisOpenEntry, parenthesisCloseEntry, innerContentContentSymbol },
				{}));

		LinqVector currentContent(targetLinkerScript->ParsedContent());
		currentContent.emplace_back(functionCall);
		targetLinkerScript->UpdateParsedContent(currentContent, {});		
	}
}

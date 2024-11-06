#ifndef COVERLAY_SECTION_STATEMENT_H_
#define COVERLAY_SECTION_STATEMENT_H_

#include "CSectionDefinitionBase.h"
#include "COverlaySectionOutput.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
	/// @brief Represents a 'Section' object found under 'Overlay' frames.
	class COverlaySectionStatement : CContentBase
	{
		DECLARE_READONLY_PROPERTY(std::string, Name)
		DECLARE_READONLY_PROPERTY(SharedPtrVector<COverlaySectionOutput>, ChildOutputs)

	protected:
		~COverlaySectionStatement() = default;

	public:
		// @brief Default constructor
		COverlaySectionStatement(bool isExternal):
			CContentBase(false, isExternal)
		{}
	};
}

#endif // COVERLAY_SECTION_STATEMENT_H_

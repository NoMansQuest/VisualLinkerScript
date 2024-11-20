#ifndef COVERLAY_SECTION_STATEMENT_H_
#define COVERLAY_SECTION_STATEMENT_H_

#include "CSectionDefinitionBase.h"
#include "COverlaySectionOutput.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
	/// @brief Represents a 'Section' object found under 'Overlay' frames.
	class COverlaySectionStatement
	{
		DECLARE_READONLY_PROPERTY(std::string, Name)
		DECLARE_READONLY_PROPERTY(SharedPtrVector<COverlaySectionOutput>, ChildContent)

	protected:
		~COverlaySectionStatement() = default;

	public:
		// @brief Default constructor
		COverlaySectionStatement(std::string name, const SharedPtrVector<COverlaySectionOutput>& childContent)
			: m_Name(std::move(name)),
			  m_ChildContent(childContent)
		{}
	};
}

#endif // COVERLAY_SECTION_STATEMENT_H_

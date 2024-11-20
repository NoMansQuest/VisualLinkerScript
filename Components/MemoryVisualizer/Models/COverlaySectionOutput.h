#ifndef COVERLAY_SECTION_OUTPUT_H__
#define COVERLAY_SECTION_OUTPUT_H__

#include "Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
	/// @brief Represents an 'Overlap' object found in "OVERLAY" statements.
	class COverlaySectionOutput
	{
		DECLARE_READONLY_PROPERTY(std::string, Content)

	protected:
		~COverlaySectionOutput() = default;

	public:

		/// @brief Default constructor.
		COverlaySectionOutput(std::string content)
			: m_Content(std::move(content))
		{}
	};
}

#endif // COVERLAY_SECTION_OUTPUT_H__

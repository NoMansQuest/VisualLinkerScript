#ifndef COVERLAY_SECTION_OUTPUT_H__
#define COVERLAY_SECTION_OUTPUT_H__

#include "CContentBase.h"
#include "Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
	/// @brief Represents an 'Overlap' object found in "OVERLAY" statements.
	class COverlaySectionOutput : CContentBase
	{
		DECLARE_READONLY_PROPERTY(std::string, Content)

	protected:
		~COverlaySectionOutput() = default;

	public:
		/// @brief Default constructor.
		COverlaySectionOutput(bool isExternal) :
			CContentBase(true, isExternal)
		{}
	};
}

#endif // COVERLAY_SECTION_OUTPUT_H__

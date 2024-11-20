#ifndef CSECTION_OUTPUT_H__
#define CSECTION_OUTPUT_H__

#include "Helpers.h"
#include "SRectangleF.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
	/// @brief Represents an 'Overlap' object found in "SECTION"
	class CSectionOutput
	{		
		DECLARE_READONLY_PROPERTY(std::string, Content)
		DECLARE_STANDARD_PROPERTY(SRectangleF, BodyArea)

	protected:
		~CSectionOutput() = default;

	public:
		/// @brief Default constructor.
		explicit CSectionOutput(std::string content) :
			m_Content(std::move(content))
		{}
	};
}

#endif // CSECTION_OUTPUT_H__s

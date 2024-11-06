#ifndef CSECTION_OUTPUT_H__
#define CSECTION_OUTPUT_H__

#include "CContentBase.h"
#include "Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
	/// @brief Represents an 'Overlap' object found in "SECTION"
	class CSectionOutput : CContentBase
	{		
		DECLARE_READONLY_PROPERTY(std::string, Content)

	protected:
		~CSectionOutput() = default;

	public:
		/// @brief Default constructor.
		CSectionOutput(bool isExternal) :
			CContentBase(true, isExternal)
		{}
	};
}

#endif // CSECTION_OUTPUT_H__s

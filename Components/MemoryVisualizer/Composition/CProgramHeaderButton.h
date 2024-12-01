#ifndef CPROGRAM_HEADER_H__
#define CPROGRAM_HEADER_H__

#include "CInteractiveObject.h"
#include "CModelMappedObject.h"
#include "Helpers.h"
#include "SMetricRectangleF.h"
#include "SMetricSizeF.h"
#include "Models/Raw/CRawEntry.h"

class QFontMetrics;

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	/// @brief Represents an 'ProgramHeader' object, found in many places
	class CProgramHeaderButton : public CModelMappedObject, public CInteractiveObject
	{
		DECLARE_READONLY_PROPERTY(std::string, ProgramHeader)		
		DECLARE_STANDARD_PROPERTY(SMetricRectangleF, ProgramHeaderArea)

		/// @brief Default constructor.
		CProgramHeaderButton(
				std::string programHeader, 
				const uint32_t inModelStartPosition, 
				const uint32_t inModelLength) :
			CModelMappedObject(inModelStartPosition, inModelLength),
			m_ProgramHeader(std::move(programHeader))
		{}

		/// Calculates the size of the area needed by the program header
		SMetricSizeF CalculateBodySize(
			const double dpiX,
			const double dpiY,
			const QFontMetrics& fontMetricsSmall,
			const QFontMetrics& fontMetricsLarge) const;

		/// Sets the allocated area where the program-header must be drawn.
		void SetBodyPosition(
			SMetricRectangleF allocatedArea,
			const double dpiX,
			const double dpiY,
			const QFontMetrics& fontMetricsSmall,
			const QFontMetrics& fontMetricsLarge);
	};
}

#endif // CPROGRAM_HEADER_H__
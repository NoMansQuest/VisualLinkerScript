#ifndef CPROGRAM_HEADER_H__
#define CPROGRAM_HEADER_H__

#include "CDrawableObjectBase.h"
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
	class CProgramHeaderButton : public CModelMappedObject, public CInteractiveObject, public CDrawableObjectBase
	{
		DECLARE_READONLY_PROPERTY(std::string, ProgramHeaderText)		
		DECLARE_STANDARD_PROPERTY(SMetricRectangleF, ProgramHeaderTextArea)
		DECLARE_STANDARD_PROPERTY(SMetricRectangleF, BodyArea)

		/// @brief Default constructor.
		CProgramHeaderButton(
				std::string programHeaderText, 
				const uint32_t inModelStartPosition, 
				const uint32_t inModelLength) :
			CModelMappedObject(inModelStartPosition, inModelLength),
			m_ProgramHeaderText(std::move(programHeaderText))
		{}

		/// @copydoc CDrawableObjectBase::Paint
		void Paint(const QPainter& painter) override;

		/// Calculates the size of the area needed by the program header
		SMetricSizeF CalculateBodySize(
			const double dpiX,
			const double dpiY,
			const QFontMetrics& fontMetricsSmall,
			const QFontMetrics& fontMetricsLarge) const;

		/// Sets the allocated area where the program-header must be drawn.
		void SetBodyPosition(
			const SMetricRectangleF& allocatedArea,
			const double dpiX,
			const double dpiY,
			const QFontMetrics& fontMetricsSmall,
			const QFontMetrics& fontMetricsLarge);
	};
}

#endif // CPROGRAM_HEADER_H__
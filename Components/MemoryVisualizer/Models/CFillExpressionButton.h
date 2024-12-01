#ifndef CFILL_EXPRESSION_H__
#define CFILL_EXPRESSION_H__

#include "CModelMappedObject.h"
#include "Helpers.h"
#include "SMetricRectangleF.h"
#include "SMetricSizeF.h"
#include "Models/Raw/CRawEntry.h"

class QFontMetrics;

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
	/// @brief Represents an 'FillExpression' object, found in many places
	class CFillExpressionButton : public CModelMappedObject
	{
		DECLARE_READONLY_PROPERTY(bool, Defined)
		DECLARE_READONLY_PROPERTY(std::string, FillExpression)
		DECLARE_STANDARD_PROPERTY(SMetricRectangleF, FillExpressionArea)

	public:
		/// @brief Default constructor.
		CFillExpressionButton(
				const bool defined,
				std::string fillExpression, 
				const uint32_t inModelStartPosition,
				const uint32_t inModelLength) :
			CModelMappedObject(inModelStartPosition, inModelLength),
			m_Defined(defined),
			m_FillExpression(std::move(fillExpression))
		{}

		/// Calculates the size of the area needed by the program header
		SMetricSizeF CalculateDesiredSize(
			const double dpiX,
			const double dpiY,
			const QFontMetrics& fontMetricsSmall,
			const QFontMetrics& fontMetricsLarge) const;

		/// Sets the allocated area where the program-header must be drawn.
		void SetGeometry(SMetricRectangleF allocatedArea);
	};
}

#endif // CFILL_EXPRESSION_H__
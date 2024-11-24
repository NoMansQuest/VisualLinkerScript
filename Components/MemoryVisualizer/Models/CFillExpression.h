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
	using namespace VisualLinkerScript::Models::Raw;

	/// @brief Represents an 'FillExpression' object, found in many places
	class CFillExpression : CModelMappedObject
	{
		DECLARE_READONLY_PROPERTY(std::string, FillExpression)
		DECLARE_STANDARD_PROPERTY(SMetricRectangleF, FillExpressionArea)

	protected:
		~CFillExpression() = default;

	public:
		/// @brief Default constructor.
		explicit CFillExpression(
				std::string fillExpression, 
				const CRawEntry& associatedRawEntry) :
			m_FillExpression(std::move(fillExpression)),
			m_AssociatedRawEntry(associatedRawEntry)
		{}

	public:
		/// Calculates the size of the area needed by the program header
		SMetricSizeF CalculateDesiredSize(const QFontMetrics& fontMetrics);

		/// Sets the allocated area where the program-header must be drawn.
		void SetGeometry(SMetricRectangleF allocatedArea);
	};
}

#endif // CFILL_EXPRESSION_H__
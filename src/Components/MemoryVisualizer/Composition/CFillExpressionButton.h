#ifndef CFILL_EXPRESSION_H__
#define CFILL_EXPRESSION_H__

#include "CDrawableObjectBase.h"
#include "CInteractiveObject.h"
#include "CModelMappedObject.h"
#include "Helpers.h"
#include "SMetricRectangleF.h"
#include "SMetricSizeF.h"
#include "Models/Raw/CRawEntry.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	class CGraphicContext;
}

class QFontMetrics;

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	/// @brief Represents an 'FillExpression' object, found in many places
	class CFillExpressionButton :
		public CModelMappedObject,
		public CDrawableObjectBase,
		public CInteractiveObject
	{
		DECLARE_READONLY_PROPERTY(bool, Defined)
		DECLARE_READONLY_PROPERTY(std::string, FillExpression)
		DECLARE_STANDARD_PROPERTY(SMetricRectangleF, FillExpressionArea)
		DECLARE_STANDARD_PROPERTY(SMetricRectangleF, BodyArea)

	public:
		/// @brief Default constructor.
		CFillExpressionButton(
				std::string fillExpression, 
				const uint32_t inModelStartPosition,
				const uint32_t inModelLength) :
			CModelMappedObject(inModelStartPosition, inModelLength),
			m_Defined(true),
			m_FillExpression(std::move(fillExpression))
		{}

		/// @brief Undefined constructor
		CFillExpressionButton() :
			CModelMappedObject(0, 0),
			m_Defined(false),
			m_FillExpression()
		{}
	
		/// @copydoc CDrawableObjectBase::Paint
		void Paint(
			const CGraphicContext& graphicContext,
			QPainter& painter) override;

		/// Calculates the size of the area needed by the program header
		SMetricSizeF CalculateBodySize(const CGraphicContext& graphicContext) const;

		/// Sets the allocated area where the program-header must be drawn.
		void SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext);
	};
}

#endif // CFILL_EXPRESSION_H__
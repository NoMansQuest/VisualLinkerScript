#ifndef COVERLAY_SECTION_STATEMENT_H_
#define COVERLAY_SECTION_STATEMENT_H_

#include "Components/MemoryVisualizer/Composition/CSectionDefinitionBase.h"
#include "Components/MemoryVisualizer/Composition/COverlaySectionOutput.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	/// @brief Represents a 'Section' object found under 'Overlay' frames.
	class COverlaySectionStatement : public CAddressedRegion
	{
		DECLARE_READONLY_PROPERTY(std::string, Title)
		DECLARE_STANDARD_PROPERTY(SMetricRectangleF, TitleArea)
		DECLARE_STANDARD_PROPERTY(SMetricRectangleF, HeaderArea)
		DECLARE_READONLY_PROPERTY(std::shared_ptr<LinqVector<COverlaySectionOutput>>, ChildContent)
		DECLARE_READONLY_PROPERTY(std::shared_ptr<LinqVector<CProgramHeaderButton>>, ProgramHeaders)
		DECLARE_READONLY_PROPERTY(CFillExpressionButton, FillExpression)
		DECLARE_READONLY_PROPERTY(std::string, LoadRegion)


	public:
		// @brief Default constructor
		COverlaySectionStatement(
				std::string title,
				const CFillExpressionButton& fillExpression,
				const std::shared_ptr<LinqVector<CProgramHeaderButton>>& programHeaders,
				const std::shared_ptr<LinqVector<COverlaySectionOutput>>& childContent,
				const uint32_t inModelStartPosition,
				const uint32_t inModelLength,
				const std::string& startAddress,
				const std::string& endAddress,
				const std::string& memorySize):
			  CAddressedRegion(
				  inModelStartPosition,
				  inModelLength,
				  startAddress, 
				  endAddress, 
				  memorySize),
			  m_Title(std::move(title)),
			  m_ChildContent(childContent),
			  m_ProgramHeaders(programHeaders),
			  m_FillExpression(fillExpression)
		{}

		/// @copydoc CDrawableObjectBase::Paint
		void Paint(
			const CGraphicContext& graphicContext,
			QPainter& painter) override;

		/// @copydoc CAddressedRegion::CalculateBodySize
		SMetricSizeF CalculateBodySize(const CGraphicContext& graphicContext) const override;

		/// @copydoc CAddressedRegion::SetBodyPosition
		void SetBodyPosition(const SMetricRectangleF& allocatedArea, const CGraphicContext& graphicContext) override;

	protected:
		/// @brief Not used for this object
		int SizeMarkerDepth() override { return 0; }
	};
}

#endif // COVERLAY_SECTION_STATEMENT_H_

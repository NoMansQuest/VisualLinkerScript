#ifndef COVERLAY_SECTION_STATEMENT_H_
#define COVERLAY_SECTION_STATEMENT_H_

#include "CSectionDefinitionBase.h"
#include "COverlaySectionOutput.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	/// @brief Represents a 'Section' object found under 'Overlay' frames.
	class COverlaySectionStatement : public CAddressedRegion
	{
		DECLARE_READONLY_PROPERTY(std::string, Name)
		DECLARE_READONLY_PROPERTY(SharedPtrVector<COverlaySectionOutput>, ChildContent)
		DECLARE_READONLY_PROPERTY(std::vector<CProgramHeaderButton>, ProgramHeaders)
		DECLARE_READONLY_PROPERTY(CFillExpressionButton, FillExpression)
		DECLARE_READONLY_PROPERTY(std::string, LoadRegion)

	public:
		// @brief Default constructor
		COverlaySectionStatement(
				std::string name,
				const CFillExpressionButton& fillExpression,
				const std::vector<CProgramHeaderButton>& programHeaders,
				const SharedPtrVector<COverlaySectionOutput>& childContent,
				const uint32_t inModelStartPosition,
				const uint32_t inModelLength,
				const bool startAddressKnown,
				const bool endAddressKnown,
				const bool memorySizeKnown):
			  CAddressedRegion(
				  inModelStartPosition,
				  inModelLength,
				  startAddressKnown, 
				  endAddressKnown, 
				  memorySizeKnown),
			  m_Name(std::move(name)),
			  m_ChildContent(childContent),
			  m_ProgramHeaders(programHeaders),
			  m_FillExpression(fillExpression)
		{}

		/// @copydoc CAddressedRegion::CalculateBodySize
		SMetricSizeF CalculateBodySize(
			const double dpiX,
			const double dpiY,
			const QFontMetrics& fontMetricsSmall,
			const QFontMetrics& fontMetricsLarge) override;

		/// @copydoc CAddressedRegion::SetBodyPosition
		void SetBodyPosition(
			SMetricRectangleF allocatedArea,
			const double dpiX,
			const double dpiY,
			const QFontMetrics& fontMetricsSmall,
			const QFontMetrics& fontMetricsLarge) override;
	};
}

#endif // COVERLAY_SECTION_STATEMENT_H_

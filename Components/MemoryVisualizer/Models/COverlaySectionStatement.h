#ifndef COVERLAY_SECTION_STATEMENT_H_
#define COVERLAY_SECTION_STATEMENT_H_

#include "CSectionDefinitionBase.h"
#include "COverlaySectionOutput.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
	/// @brief Represents a 'Section' object found under 'Overlay' frames.
	class COverlaySectionStatement : public CAddressedRegion
	{
		DECLARE_READONLY_PROPERTY(std::string, Name)
		DECLARE_READONLY_PROPERTY(SharedPtrVector<COverlaySectionOutput>, ChildContent)
		DECLARE_READONLY_PROPERTY(std::vector<std::string>, programHeaders)
		DECLARE_READONLY_PROPERTY(std::string, fillExpression)
		DECLARE_READONLY_PROPERTY(std::string, loadRegion)

		// @brief Default constructor
		COverlaySectionStatement(std::string name, const SharedPtrVector<COverlaySectionOutput>& childContent)
			: m_Name(std::move(name)),
			  m_ChildContent(childContent)
		{}

		/// @copydoc CAddressedRegion::CalculateDesiredSize
		SMetricSizeF CalculateDesiredSize(const QFontMetrics& fontMetrics) override;

		/// @copydoc CAddressedRegion::SetGeometry
		void SetGeometry(SMetricRectangleF allocatedArea) override;
	};
}

#endif // COVERLAY_SECTION_STATEMENT_H_

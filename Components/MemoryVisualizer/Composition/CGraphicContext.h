#ifndef CGRAPHIC_CONTEXT_H__
#define CGRAPHIC_CONTEXT_H__

#include <QWidget>
#include <QFontMetrics>
#include <QFont>
#include <qscreen.h>
#include "Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	/// @brief Object contains information needed for graphics related calculations.
	class CGraphicContext
	{
	private:
		double m_dpiX;
		double m_dpiY;
		QFontMetrics m_fontMetricsSmall;
		QFontMetrics m_fontMetricsLarge;
		QFont m_fontSmall;
		QFont m_fontSmallBold;
		QFont m_fontLarge;
		QFont m_fontLargeBold;

	public:
		/// @brief Default constructor
		CGraphicContext(
				double dpiX, 
				double dpiY, 
				const QFont& fontSmall,
				const QFont& fontSmallBold,
				const QFont& fontLarge,
				const QFont& fontLargeBold) :
			m_dpiX(dpiX),
			m_dpiY(dpiY),
			m_fontMetricsSmall(QFontMetrics(fontSmall)),
			m_fontMetricsLarge(QFontMetrics(fontLarge)),
			m_fontSmall(fontSmall),
			m_fontSmallBold(fontSmallBold),
			m_fontLarge(fontLarge),
			m_fontLargeBold(fontLargeBold)
		{}
	
		///	@brief Reports back the DPI-X.
		[[nodiscard]] double DpiX() const { return this->m_dpiX; }

		///	@brief Reports back the DPI-Y.
		[[nodiscard]] double DpiY() const { return this->m_dpiY; }

		///	@brief Reports back the DPI-X.
		[[nodiscard]] double DpmX() const { return Graphical::GetDpmFromDpi(this->m_dpiX); }

		///	@brief Reports back the DPI-Y.
		[[nodiscard]] double DpmY() const { return Graphical::GetDpmFromDpi(this->m_dpiY); }

		///	@brief Reports back the FontMetrics (small).
		QFontMetrics FontMetricsSmall() const { return this->m_fontMetricsSmall; }

		///	@brief Reports back the FontMetrics (Large).
		QFontMetrics FontMetricsLarge() const { return this->m_fontMetricsLarge; }

		///	@brief Reports back the small font.
		QFont FontSmall() const { return this->m_fontSmall; }

		///	@brief Reports back the small font in bold.
		QFont FontSmallBold() const { return this->m_fontSmall; }

		///	@brief Reports back the large font.
		QFont FontLarge() const { return this->m_fontLarge; }

		///	@brief Reports back the large font in bold.
		QFont FontLargeBold() const { return this->m_fontLargeBold; }

		/// @brief Constructs a GraphicalContext for the given widget
		static CGraphicContext Make(const QWidget* targetWidget)
		{
			auto dpiX = targetWidget->screen()->physicalDotsPerInchX();
			auto dpiY = targetWidget->screen()->physicalDotsPerInchY();

			auto smallFontSize = Graphical::GetFontSizeFromMetric(dpiY, 2.3);
			auto largeFontSize = Graphical::GetFontSizeFromMetric(dpiY, 3.5);

			QFont smallFont("Tahoma, DejaVu Sans", smallFontSize);
			smallFont.setFamily("Arial, Helvetica, sans-serif"); // Specify primary and fallback fonts

			QFont smallFontBold("Tahoma, DejaVu Sans", smallFontSize);
			smallFontBold.setBold(true);
			smallFontBold.setFamily("Arial, Helvetica, sans-serif"); // Specify primary and fallback fonts

			QFont largeFont("Tahoma, DejaVu Sans", largeFontSize);
			largeFont.setFamily("Arial, Helvetica, sans-serif"); // Specify primary and fallback fonts

			QFont largeFontBold("Tahoma, DejaVu Sans", largeFontSize);
			largeFontBold.setBold(true);
			largeFontBold.setFamily("Arial, Helvetica, sans-serif"); // Specify primary and fallback fonts

			return CGraphicContext(dpiX, dpiY, smallFont, smallFontBold, largeFont, largeFontBold);
		}

	};
}


#endif


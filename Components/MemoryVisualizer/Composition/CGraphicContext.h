#ifndef CGRAPHIC_CONTEXT_H__
#define CGRAPHIC_CONTEXT_H__

#include <QFontMetrics>
#include <QFont>

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
	};
}


#endif


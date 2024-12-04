#ifndef CGRAPHIC_CONTEXT_H__
#define CGRAPHIC_CONTEXT_H__

#include <QFontMetrics>

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

	public:
		/// @brief Default constructor
		CGraphicContext(
				double dpiX, 
				double dpiY, 
				const QFontMetrics& fontMetricsSmall,
				const QFontMetrics& fontMetricsLarge):
			m_dpiX(dpiX),
			m_dpiY(dpiY),
			m_fontMetricsSmall(fontMetricsSmall),
			m_fontMetricsLarge(fontMetricsLarge)
		{}
	
		///	@brief Reports back the DPI-X.
		[[nodiscard]] double DpiX() const { return this->m_dpiX; }

		///	@brief Reports back the DPI-Y.
		[[nodiscard]] double DpiY() const { return this->m_dpiY; }

		///	@brief Reports back the FontMetrics (small).
		const QFontMetrics& FontMetricsSmall() { return this->m_fontMetricsSmall; }

		///	@brief Reports back the FontMetrics (Large).
		const QFontMetrics& FontMetricsLarge() { return this->m_fontMetricsLarge; }

	};
}


#endif


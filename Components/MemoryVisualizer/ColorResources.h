#ifndef COLOR_RESOURCES_H__
#define COLOR_RESOURCES_H__
#include <QColor>

namespace VisualLinkerScript::Components::MemoryVisualizer::Colors
{	
	// Default background
	constexpr QRgb ViewBackgroundColor = qRgba(0x18, 0x18, 0x18, 0xff);
	constexpr QRgb ViewBackgroundGridColor = qRgba();

	// Address markers
	constexpr QRgb AddressMarkerLineColor = qRgba(0x7d, 0x7d, 0x7d, 0xff);
	constexpr QRgb AddressMarkerTextColor = qRgba(0xff, 0xff, 0xff, 0xff);

	// Size markers
	constexpr QRgb SizeMarkerLineColor = qRgba(0x00, 0x7d, 0x99, 0xff);
	constexpr QRgb SizeMarkerTextColor = qRgba(0xff, 0xff, 0xff, 0xff);

	// FillExpression
	constexpr QRgb FillExpressionDefaultBackground = qRgba(0x00, 0x00, 0x00, 0x47);
	constexpr QRgb FillExpressionHoverBackground = qRgba(0x00, 0x00, 0x00, 0x27);
	constexpr QRgb FillExpressionClickBackground = qRgba(0x00, 0x00, 0x00, 0x4D);
				   
	constexpr QRgb FillExpressionDefaultBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
	constexpr QRgb FillExpressionHoverBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
	constexpr QRgb FillExpressionClickBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
				   
	constexpr QRgb FillExpressionTextDefaultForeColor = qRgba(0xff, 0xff, 0xff, 0xff);
	constexpr QRgb FillExpressionTextHoverForeColor = qRgba(0xff, 0xff, 0xff, 0xff);
	constexpr QRgb FillExpressionTextClickForeColor = qRgba(0xff, 0xff, 0xff, 0xff);

	// ProgramHeader
	constexpr QRgb ProgramHeaderDefaultBackground = qRgba(0xfa, 0xff, 0x00, 0x47);
	constexpr QRgb ProgramHeaderHoverBackground = qRgba(0xfa, 0xff, 0x00, 0x57);
	constexpr QRgb ProgramHeaderClickBackground = qRgba(0xfa, 0xff, 0x00, 0x4C);
				   
	constexpr QRgb ProgramHeaderDefaultBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
	constexpr QRgb ProgramHeaderHoverBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
	constexpr QRgb ProgramHeaderClickBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
				   
	constexpr QRgb ProgramHeaderTextDefaultForeColor = qRgba(0xff, 0xff, 0xff, 0xff);
	constexpr QRgb ProgramHeaderTextHoverForeColor = qRgba(0xff, 0xff, 0xff, 0xff);
	constexpr QRgb ProgramHeaderTextClickForeColor = qRgba(0xff, 0xff, 0xff, 0xff);

	// Memory Region
	constexpr QRgb MemoryRegionDefaultBackgroundColor = qRgba(0xff, 0xff, 0xff, 0x11);
	constexpr QRgb MemoryRegionHoverBackgroundColor = qRgba(0xff, 0xff, 0xff, 0x20);
	constexpr QRgb MemoryRegionClickBackgroundColor = qRgba(0xff, 0xff, 0xff, 0x19);
						 
	constexpr QRgb MemoryRegionDefaultBorderColor = qRgba(0xff, 0xff, 0xff, 0x20);
	constexpr QRgb MemoryRegionHoverBorderColor = qRgba(0xff, 0xff, 0xff, 0x20);
	constexpr QRgb MemoryRegionClickBorderColor = qRgba(0xff, 0xff, 0xff, 0x20);
						 
	constexpr QRgb MemoryRegionDefaultForeColor = qRgba(0xff, 0xff, 0xff, 0xff);
	constexpr QRgb MemoryRegionHoverForeColor = qRgba(0xff, 0xff, 0xff, 0xff);
	constexpr QRgb MemoryRegionClickForeColor = qRgba(0xff, 0xff, 0xff, 0xff);

	// Overlay Statement
	constexpr QRgb OverlayHeaderBackgroundColor = qRgba(0x00, 0xe4, 0x17, 0x23);

	constexpr QRgb OverlayStatementDefaultBackgroundColor = qRgba(0xd4, 0xa8, 0x0c, 0x11);
	constexpr QRgb OverlayStatementHoverBackgroundColor = qRgba(0xd4, 0xa8, 0x0c, 0x20);
	constexpr QRgb OverlayStatementClickBackgroundColor = qRgba(0xd4, 0xa8, 0x0c, 0x19);

	constexpr QRgb OverlayStatementDefaultBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
	constexpr QRgb OverlayStatementHoverBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
	constexpr QRgb OverlayStatementClickBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);

	constexpr QRgb OverlayStatementDefaultForeColor = qRgba(0xff, 0xff, 0xff, 0xff);
	constexpr QRgb OverlayStatementHoverForeColor = qRgba(0xff, 0xff, 0xff, 0xff);
	constexpr QRgb OverlayStatementClickForeColor = qRgba(0xff, 0xff, 0xff, 0xff);

	// Section (including both normal section and section placed in overlay)
	constexpr QRgb SectionStatementHeaderBackgroundColor = qRgba(0x00, 0xe4, 0x17, 0x23);

	constexpr QRgb SectionStatementDefaultBackgroundColor = qRgba(0xff, 0xff, 0xff, 0x11);
	constexpr QRgb SectionStatementHoverBackgroundColor = qRgba(0xff, 0xff, 0xff, 0x20);
	constexpr QRgb SectionStatementClickBackgroundColor = qRgba(0xff, 0xff, 0xff, 0x19);
				   
	constexpr QRgb SectionStatementDefaultBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
	constexpr QRgb SectionStatementHoverBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
	constexpr QRgb SectionStatementClickBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
				   
	constexpr QRgb SectionStatementDefaultForeColor = qRgba(0xff, 0xff, 0xff, 0xff);
	constexpr QRgb SectionStatementHoverForeColor = qRgba(0xff, 0xff, 0xff, 0xff);
	constexpr QRgb SectionStatementClickForeColor = qRgba(0xff, 0xff, 0xff, 0xff);

	// Section Output Statement 
	constexpr QRgb SectionOutputDefaultBackgroundColor = qRgba(0xff, 0xff, 0xff, 0x11);
	constexpr QRgb SectionOutputHoverBackgroundColor = qRgba(0xff, 0xff, 0xff, 0x20);
	constexpr QRgb SectionOutputClickBackgroundColor = qRgba(0xff, 0xff, 0xff, 0x19);
				   
	constexpr QRgb SectionOutputDefaultBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
	constexpr QRgb SectionOutputHoverBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
	constexpr QRgb SectionOutputClickBorderColor = qRgba(0xff, 0xff, 0xff, 0x00);
				   
	constexpr QRgb SectionOutputDefaultForeColor = qRgba(0xff, 0xff, 0xff, 0xff);
	constexpr QRgb SectionOutputHoverForeColor = qRgba(0xff, 0xff, 0xff, 0xff);
	constexpr QRgb SectionOutputClickForeColor = qRgba(0xff, 0xff, 0xff, 0xff);
}


#endif


#ifndef CINTERACTIVE_OBJECT_H__
#define CINTERACTIVE_OBJECT_H__

#include "SMetricPointF.h"
#include "../../Helpers.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	/// @brief Represents any object that responds to mouse action (hovering, clicking, etc.)
	class CInteractiveObject 
	{
		DECLARE_STANDARD_PROPERTY(bool, MouseDown)
		DECLARE_STANDARD_PROPERTY(bool, MouseHovering)

		// The exact local coordinates in the widget where the mouse click took place.
		DECLARE_STANDARD_PROPERTY(SMetricPointF, LocalMouseDownOrigin)

		// The exact screen coordinates in the widget where the mouse click took place.
		DECLARE_STANDARD_PROPERTY(SMetricPointF, ScreenMouseDownOrigin)

		/// @brief Default constructor.
		explicit CInteractiveObject() = default;

	};
}

#endif // CINTERACTIVE_OBJECT_H__
#ifndef CDRAWABLE_OBJECT_BASE_H__
#define CDRAWABLE_OBJECT_BASE_H__

class QPainter;

namespace VisualLinkerScript::Components::MemoryVisualizer::Composition
{
	/// @brief Object represents the base-class for all elements that could
	///		   be drawn on the screen.
	class CDrawableObjectBase
	{

	public:
		/// @brief Default destructor
		virtual ~CDrawableObjectBase() = default;

		/// @brief In charge of drawing the object on the painter.
		virtual void Paint(const QPainter& painter) = 0;
	};	
}

#endif

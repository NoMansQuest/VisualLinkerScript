#ifndef CSIZEINDICATOR_H
#define CSIZEINDICATOR_H

#include <string>
#include "CContentBase.h"

namespace VisualLinkerScript::Components::MemoryVisualizer::Models
{
    /// @brief Represents the "ENTRY()" directive.
    class CSizeIndicator : public CContentBase
    {
        /**
         *  Desceription
         *  ==============================
         *
         *  $-----o  ----+ ( Top Bracket Right Point )
         *  $     |      |
         *  $     |      |
         *  $     |      |
         *  $     |      +----X < To be determined >  ( X: Text Center Left Point )
         *  $     |      |
         *  $     |      |
         *  $     |      |
         *  $-----o  ----+ ( Bottom Bracket Right Point )
         */

        DECLARE_STANDARD_PROPERTY( SPointF, TopBracketRightPoint )
        DECLARE_STANDARD_PROPERTY( SPointF, BottomBracketRightPoint )
        DECLARE_STANDARD_PROPERTY( SPointF, CurlyBracketCenterPoint )
        DECLARE_STANDARD_PROPERTY( SPointF, TextIndicaterCenterLeftPoint )
        DECLARE_STANDARD_PROPERTY( std::string, DisplayValue )

    public:
        /// @brief Default contructor
        CSizeIndicator(bool isDraggable, bool isExternal) :
            CContentBase(isDraggable, isExternal)
        {}
    };
};


#endif // CSIZEINDICATOR_H

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

    // Member fields
    private:
        SPointF m_topBracketRightPoint;
        SPointF m_bottomBracketRightPoint;
        SPointF m_curlyBracketCenterPoint;
        SPointF m_textIndicaterCenterLeftPoint;
        std::string m_displayValue;

    public:
        /// @brief Default contructor
        CSizeIndicator(bool isDraggable, bool isExternal) :
            CContentBase(isDraggable, isExternal)
        {}

        /// @brief Default destructor
        ~CSizeIndicator()
        {}

    public:
        /// @brief Returns Top bracket right point
        SPointF TopBracketRightPoint(){
            return this->m_topBracketRightPoint;
        }

        /// @brief Updates top bracket right point
        void SetTopBracketRightPoint(SPointF value){
            this->m_topBracketRightPoint = value;
        }

        /// @brief Return Bottom Bracket Right Point
        SPointF BottomBracketRightPoint(){
            return this->m_bottomBracketRightPoint;
        }

        /// @brief Update Bottom Bracket Right Point
        void SetBottomBracketRightPoint(SPointF value){
            this->m_bottomBracketRightPoint = value;
        }

        /// @brief Return Curly Bracket Center Point
        SPointF CurlyBracketCenterPoint(){
            return this->m_curlyBracketCenterPoint;
        }

        /// @brief Update Curly Bracket Cetner Point
        void SetCurlyBracketCenterPoint(SPointF value){
            m_curlyBracketCenterPoint = value;
        }

        /// @brief Return Text Indicator Center Left Poisition
        SPointF TextIndicatorCenterLeftPoint(){
            return this->m_textIndicaterCenterLeftPoint;
        }

        /// @brief Updates Text Indicator Center Left Point
        void SetTextIndicatorCenterLeftPoint(SPointF value){
            m_textIndicaterCenterLeftPoint = value;
        }

        /// @brief Updates Display Value
        void SetDisplayValue(std::string newValue){
            this->m_displayValue = newValue;
        }

        /// @brief Returns Diplay Value
        std::string DisplayValue(){
            return this->m_displayValue;
        }
    };
};


#endif // CSIZEINDICATOR_H

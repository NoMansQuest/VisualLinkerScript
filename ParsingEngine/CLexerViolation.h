#ifndef CLEXER_VIOLATION_H_
#define CLEXER_VIOLATION_H_

#include "ELexerViolationCode.h"
#include "../Models/CViolationBase.h"

namespace VisualLinkerScript::ParsingEngine
{
    using namespace VisualLinkerScript::Models;

    /// @brief This object contains information about a detected violation in the Linker Script
    class CLexerViolation : public CViolationBase
    {
    private:
        ELexerViolationCode m_violationCode;
        CRawEntry m_violationLocation;        

    public:
        /// @brief Default constructor
        explicit CLexerViolation(CRawEntry violationLocation, ELexerViolationCode violationCode)
            : m_violationCode(violationCode),
              m_violationLocation(violationLocation)
        {}

        /// @brief Reports back the location of the violation.
        const CRawEntry ViolationLocation()
        {
            return this->m_violationLocation;
        }

        /// @brief Reports back the tpe of violation        
        const ELexerViolationCode Code()
        {
            return m_violationCode;
        }

        /// @brief Type of violation.
        virtual EViolationType Type() override {
            return EViolationType::LexerViolation;
        }
    };
}

#endif // CPARSERVIOLATION_H__

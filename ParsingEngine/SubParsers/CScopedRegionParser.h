#ifndef CSCOPED_REGION_PARSER_H__
#define CSCOPED_REGION_PARSER_H__

#include "CSubParserBase.h"
#include "SubParserType.h"
#include <memory>
#include <type_traits>

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Parser in charge of parsing scoped regions (such as MEMORY, PHDRS, SECTION, etc...)
    template <SubParserType TParserType, typename TContentParserType, class TProducingOutputType>
    class CScopedRegionParser : public CSubParserBase
    {
    private:
        static_assert(
            std::is_base_of_v<CSubParserBase, TContentParserType>,
            "Only parsers based on 'CSubParserBase' are allowed" );

        static_assert(
            std::is_base_of_v<CLinkerScriptContentBase, TProducingOutputType>,
            "Only 'Producing Output Types' based on 'CLinkerScriptContentBase' are allowed" );

        TContentParserType m_regionContentParser;

    protected:
        /// @brief This entry will be used by super class to determine the name of this scoped region.
        virtual std::string GetHeaderName() = 0;

    public:
        /// @copydoc CSubParserBase::Type()
        virtual SubParserType Type() override
        {
            return TParserType;
        }

        /// @copydoc CSubParserBase::TryParse(std::vector<CRawEntry>::const_iterator&)
        virtual std::shared_ptr<CLinkerScriptContentBase> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}


#endif

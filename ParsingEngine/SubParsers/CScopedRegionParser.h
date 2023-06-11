#ifndef CSCOPED_REGION_PARSER_H__
#define CSCOPED_REGION_PARSER_H__

#include "CSubParserBase.h"

namespace VisualLinkerScript::ParsingEngine::SubParsers
{
    /// @brief Parser in charge of parsing scoped regions (such as MEMORY, PHDRS, SECTION, etc...)
    template <SubParserType TParserType, typename TContentParserType, class TProducingOutputType>
    class CScopedRegionParser : public CSubParserBase<TProducingOutputType>
    {
    private:        
        static_assert(
            std::is_default_constructible< TContentParserType >::value,
            "Content-parser must be default-constructible." );

        static_assert(
            std::is_base_of< CLinkerScriptContentBase, TProducingOutputType >::value,
            "Only 'Producing Output Types' based on 'CLinkerScriptContentBase' are allowed" );

        static_assert(
            std::is_constructible< TProducingOutputType, CRawEntry, CRawEntry, CRawEntry,
                                   std::vector<std::shared_ptr<CLinkerScriptContentBase>>&&,
                                   std::vector<CRawEntry>&&,
                                   std::vector<CViolationBase>&& >::value,
            "TProducingOutputType is not compatible. Please check the constructor." );

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
        virtual std::shared_ptr<TProducingOutputType> TryParse(
                CRawFile& linkerScriptFile,
                std::vector<CRawEntry>::const_iterator& iterator,
                std::vector<CRawEntry>::const_iterator& endOfVectorIterator) override;
    };
}

#include "CScopedRegionParser.tpp"

#endif

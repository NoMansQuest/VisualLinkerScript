QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Compiler/CCompiler.cpp \
    Compiler/ExpressionEvaluation/CExpressionEvaluator.cpp \
    Components/MemoryVisualizer/CGraphicsProcessor.cpp \
    Components/MemoryVisualizer/CMemoryVisualizer.cpp \
    Components/MemoryVisualizer/CMouseInteractivity.cpp \
    Components/MemoryVisualizer/ModelTranslator.cpp \
    Components/MemoryVisualizer/Placement/CPlacementEngine.cpp \
    Components/MemoryVisualizer/Rendering/CArrowsRenderer.cpp \
    Components/MemoryVisualizer/Rendering/CMemoryObjectRenderer.cpp \
    Components/MemoryVisualizer/Rendering/COverlapObjectRenderer.cpp \
    Components/MemoryVisualizer/Rendering/CSectionObjectRenderer.cpp \
    Components/MemoryVisualizer/Rendering/CStatementRenderer.cpp \
    Components/MemoryVisualizer/Rendering/CTooltipRenderer.cpp \
    Components/QScintilla/ComponentHelpers.cpp \
    Components/QScintilla/scintilla/lexers/LexCPP.cpp \
    Components/QScintilla/scintilla/lexers/LexLinkerScript.cpp \
    Components/QScintilla/scintilla/lexlib/Accessor.cpp \
    Components/QScintilla/scintilla/lexlib/CharacterCategory.cpp \
    Components/QScintilla/scintilla/lexlib/CharacterSet.cpp \
    Components/QScintilla/scintilla/lexlib/DefaultLexer.cpp \
    Components/QScintilla/scintilla/lexlib/LexerBase.cpp \
    Components/QScintilla/scintilla/lexlib/LexerModule.cpp \
    Components/QScintilla/scintilla/lexlib/LexerNoExceptions.cpp \
    Components/QScintilla/scintilla/lexlib/LexerSimple.cpp \
    Components/QScintilla/scintilla/lexlib/PropSetSimple.cpp \
    Components/QScintilla/scintilla/lexlib/StyleContext.cpp \
    Components/QScintilla/scintilla/lexlib/WordList.cpp \
    Components/QScintilla/scintilla/src/AutoComplete.cpp \
    Components/QScintilla/scintilla/src/CallTip.cpp \
    Components/QScintilla/scintilla/src/CaseConvert.cpp \
    Components/QScintilla/scintilla/src/CaseFolder.cpp \
    Components/QScintilla/scintilla/src/Catalogue.cpp \
    Components/QScintilla/scintilla/src/CellBuffer.cpp \
    Components/QScintilla/scintilla/src/CharClassify.cpp \
    Components/QScintilla/scintilla/src/ContractionState.cpp \
    Components/QScintilla/scintilla/src/DBCS.cpp \
    Components/QScintilla/scintilla/src/Decoration.cpp \
    Components/QScintilla/scintilla/src/Document.cpp \
    Components/QScintilla/scintilla/src/EditModel.cpp \
    Components/QScintilla/scintilla/src/EditView.cpp \
    Components/QScintilla/scintilla/src/Editor.cpp \
    Components/QScintilla/scintilla/src/ExternalLexer.cpp \
    Components/QScintilla/scintilla/src/Indicator.cpp \
    Components/QScintilla/scintilla/src/KeyMap.cpp \
    Components/QScintilla/scintilla/src/LineMarker.cpp \
    Components/QScintilla/scintilla/src/MarginView.cpp \
    Components/QScintilla/scintilla/src/PerLine.cpp \
    Components/QScintilla/scintilla/src/PositionCache.cpp \
    Components/QScintilla/scintilla/src/RESearch.cpp \
    Components/QScintilla/scintilla/src/RunStyles.cpp \
    Components/QScintilla/scintilla/src/ScintillaBase.cpp \
    Components/QScintilla/scintilla/src/Selection.cpp \
    Components/QScintilla/scintilla/src/Style.cpp \
    Components/QScintilla/scintilla/src/UniConversion.cpp \
    Components/QScintilla/scintilla/src/ViewStyle.cpp \
    Components/QScintilla/scintilla/src/XPM.cpp \
    Components/QScintilla/src/InputMethod.cpp \
    Components/QScintilla/src/ListBoxQt.cpp \
    Components/QScintilla/src/MacPasteboardMime.cpp \
    Components/QScintilla/src/PlatQt.cpp \
    Components/QScintilla/src/SciAccessibility.cpp \
    Components/QScintilla/src/SciClasses.cpp \
    Components/QScintilla/src/ScintillaQt.cpp \
    Components/QScintilla/src/qsciabstractapis.cpp \
    Components/QScintilla/src/qsciapis.cpp \
    Components/QScintilla/src/qscicommand.cpp \
    Components/QScintilla/src/qscicommandset.cpp \
    Components/QScintilla/src/qscidocument.cpp \
    Components/QScintilla/src/qscilexer.cpp \
    Components/QScintilla/src/qscilexercpp.cpp \
    Components/QScintilla/src/qscilexerlinkerscript.cpp \
    Components/QScintilla/src/qscimacro.cpp \
    Components/QScintilla/src/qsciprinter.cpp \
    Components/QScintilla/src/qsciscintilla.cpp \
    Components/QScintilla/src/qsciscintillabase.cpp \
    Components/QScintilla/src/qscistyle.cpp \
    Components/QScintilla/src/qscistyledtext.cpp \
    DrcEngine/CDrcManager.cpp \
    DrcEngine/Rules/CDirectivesDeclaredOnlyOnceRule.cpp \
    DrcEngine/Rules/CEntryIsDefinedRule.cpp \
    DrcEngine/Rules/CIncludeRecursionGuard.cpp \
    DrcEngine/Rules/CIncludedFilesArePresentRule.cpp \
    DrcEngine/Rules/CLocationCounterCannotGoBackwardsRule.cpp \
    DrcEngine/Rules/CMemoryRegionDefinedOnlyOnceRule.cpp \
    DrcEngine/Rules/CNoDuplicateMemoryRegionNameRule.cpp \
    DrcEngine/Rules/CSectionsDefinedOnlyOnceRule.cpp \
    DrcEngine/Rules/CSymbolsDefinedInAdvanceRule.cpp \
    Helpers.cpp \
    LinkerScriptManager/CLinkerScriptManager.cpp \
    LinkerScriptManager/CUndoRedoManager.cpp \
    Models/CAssignmentProcedureStatement.cpp \
    Models/CAssignmentStatement.cpp \
    Models/CExpression.cpp \
    Models/CFunctionCall.cpp \
    Models/CInputSectionFunction.cpp \
    Models/CInputSectionStatement.cpp \
    Models/CLinkerScriptContentBase.cpp \
    Models/CLinkerScriptFile.cpp \
    Models/CMemoryRegion.cpp \
    Models/CMemoryStatement.cpp \
    Models/CPhdrsRegion.cpp \
    Models/CPhdrsStatement.cpp \
    Models/CProcedureCall.cpp \
    Models/CSectionOutputCommand.cpp \
    Models/CSectionOutputOverlay.cpp \
    Models/CSectionOverlayCommand.cpp \
    Models/CSectionsRegion.cpp \
    Models/CVersionScope.cpp \
    ParsingEngine/CMasterParser.cpp \
    ParsingEngine/CLexer.cpp \
    ParsingEngine/EParserViolationCode.cpp \
    ParsingEngine/SubParsers/CAssignmentParser.cpp \
    ParsingEngine/SubParsers/CAssignmentProcedureParser.cpp \
    ParsingEngine/SubParsers/CExpressionParser.cpp \
    ParsingEngine/SubParsers/CFunctionParser.cpp \
    ParsingEngine/SubParsers/CInputSectionFunctionExcludeFileParser.cpp \
    ParsingEngine/SubParsers/CInputSectionFunctionKeepParser.cpp \
    ParsingEngine/SubParsers/CInputSectionFunctionSortParser.cpp \
    ParsingEngine/SubParsers/CInputSectionStatementParser.cpp \
    ParsingEngine/SubParsers/CMemoryRegionContentParser.cpp \
    ParsingEngine/SubParsers/CMemoryStatementAttributeParser.cpp \
    ParsingEngine/SubParsers/CPhdrsRegionContentParser.cpp \
    ParsingEngine/SubParsers/CSectionOutputCommandParser.cpp \
    ParsingEngine/SubParsers/CSectionOverlayParser.cpp \
    ParsingEngine/SubParsers/CSectionsRegionParser.cpp \
    ParsingEngine/SubParsers/CSubParserBase.cpp \
    ParsingEngine/SubParsers/CVersionRegionContentParser.cpp \
    ParsingEngine/SubParsers/Constants.cpp \
    ParsingEngine/SubParsers/SubParserHelpers.cpp \
    QueryEngine/QueryCenter.tpp \    
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Compiler/CCompiler.h \
    Compiler/ExpressionEvaluation/ArithmeticOperationType.h \
    Compiler/ExpressionEvaluation/CArithmeticResultOp.h \
    Compiler/ExpressionEvaluation/CCompoundResult.h \
    Compiler/ExpressionEvaluation/CEvaluatableBase.h \
    Compiler/ExpressionEvaluation/CExpressionEvaluator.h \
    Compiler/ExpressionEvaluation/CNumericResult.h \
    Compiler/ExpressionEvaluation/CResultBase.h \
    Compiler/ExpressionEvaluation/CSymbolResult.h \
    Compiler/ExpressionEvaluation/EvaluationResultType.h \
    Compiler/FunctionEvaluation/CFunctionEvaluatorBase.h \
    Compiler/ProcedureExecution/CProcedureExecutorBase.h \
    Components/MemoryVisualizer/CGraphicsProcessor.h \
    Components/MemoryVisualizer/CMemoryVisualizer.h \
    Components/MemoryVisualizer/CMouseInteractivity.h \
    Components/MemoryVisualizer/EInsertObjectType.h \
    Components/MemoryVisualizer/ModelTranslator.h \
    Components/MemoryVisualizer/Models/CArrowObject.h \
    Components/MemoryVisualizer/Models/CButton.h \
    Components/MemoryVisualizer/Models/CComboBox.h \
    Components/MemoryVisualizer/Models/CContentBase.h \
    Components/MemoryVisualizer/Models/CDragState.h \
    Components/MemoryVisualizer/Models/CFloorplan.h \
    Components/MemoryVisualizer/Models/CLmaToVmaArrow.h \
    Components/MemoryVisualizer/Models/CLmaToVmaArrowCoordinates.h \
    Components/MemoryVisualizer/Models/CMemoryLocationIndicator.h \
    Components/MemoryVisualizer/Models/CMemoryObject.h \
    Components/MemoryVisualizer/Models/CMemoryStatement.h \
    Components/MemoryVisualizer/Models/COverlayObject.h \
    Components/MemoryVisualizer/Models/COverlaySectionObject.h \
    Components/MemoryVisualizer/Models/CSectionObject.h \
    Components/MemoryVisualizer/Models/CSizableObject.h \
    Components/MemoryVisualizer/Models/CSizeIndicator.h \
    Components/MemoryVisualizer/Models/CStatementObject.h \
    Components/MemoryVisualizer/Models/CTooltip.h \
    Components/MemoryVisualizer/Models/EObjectState.h \
    Components/MemoryVisualizer/Models/SPointF.h \
    Components/MemoryVisualizer/Models/SRectangleF.h \
    Components/MemoryVisualizer/Models/CEntryPoint.h \
    Components/MemoryVisualizer/Placement/CPlacementEngine.h \
    Components/MemoryVisualizer/Rendering/CArrowsRenderer.h \
    Components/MemoryVisualizer/Rendering/CMemoryObjectRenderer.h \
    Components/MemoryVisualizer/Rendering/COverlapObjectRenderer.h \
    Components/MemoryVisualizer/Rendering/CSectionObjectRenderer.h \
    Components/MemoryVisualizer/Rendering/CStatementRenderer.h \
    Components/MemoryVisualizer/Rendering/CTooltipRenderer.h \
    Components/MemoryVisualizer/SBackendResponse.h \
    Components/QScintilla/ComponentHelpers.h \
    Components/QScintilla/scintilla/include/ILexer.h \
    Components/QScintilla/scintilla/include/ILoader.h \
    Components/QScintilla/scintilla/include/Platform.h \
    Components/QScintilla/scintilla/include/SciLexer.h \
    Components/QScintilla/scintilla/include/Sci_Position.h \
    Components/QScintilla/scintilla/include/Scintilla.h \
    Components/QScintilla/scintilla/include/ScintillaWidget.h \
    Components/QScintilla/scintilla/lexlib/Accessor.h \
    Components/QScintilla/scintilla/lexlib/CharacterCategory.h \
    Components/QScintilla/scintilla/lexlib/CharacterSet.h \
    Components/QScintilla/scintilla/lexlib/DefaultLexer.h \
    Components/QScintilla/scintilla/lexlib/LexAccessor.h \
    Components/QScintilla/scintilla/lexlib/LexerBase.h \
    Components/QScintilla/scintilla/lexlib/LexerModule.h \
    Components/QScintilla/scintilla/lexlib/LexerNoExceptions.h \
    Components/QScintilla/scintilla/lexlib/LexerSimple.h \
    Components/QScintilla/scintilla/lexlib/OptionSet.h \
    Components/QScintilla/scintilla/lexlib/PropSetSimple.h \
    Components/QScintilla/scintilla/lexlib/SparseState.h \
    Components/QScintilla/scintilla/lexlib/StringCopy.h \
    Components/QScintilla/scintilla/lexlib/StyleContext.h \
    Components/QScintilla/scintilla/lexlib/SubStyles.h \
    Components/QScintilla/scintilla/lexlib/WordList.h \
    Components/QScintilla/scintilla/src/AutoComplete.h \
    Components/QScintilla/scintilla/src/CallTip.h \
    Components/QScintilla/scintilla/src/CaseConvert.h \
    Components/QScintilla/scintilla/src/CaseFolder.h \
    Components/QScintilla/scintilla/src/Catalogue.h \
    Components/QScintilla/scintilla/src/CellBuffer.h \
    Components/QScintilla/scintilla/src/CharClassify.h \
    Components/QScintilla/scintilla/src/ContractionState.h \
    Components/QScintilla/scintilla/src/DBCS.h \
    Components/QScintilla/scintilla/src/Decoration.h \
    Components/QScintilla/scintilla/src/Document.h \
    Components/QScintilla/scintilla/src/EditModel.h \
    Components/QScintilla/scintilla/src/EditView.h \
    Components/QScintilla/scintilla/src/Editor.h \
    Components/QScintilla/scintilla/src/ElapsedPeriod.h \
    Components/QScintilla/scintilla/src/ExternalLexer.h \
    Components/QScintilla/scintilla/src/FontQuality.h \
    Components/QScintilla/scintilla/src/Indicator.h \
    Components/QScintilla/scintilla/src/IntegerRectangle.h \
    Components/QScintilla/scintilla/src/KeyMap.h \
    Components/QScintilla/scintilla/src/LineMarker.h \
    Components/QScintilla/scintilla/src/MarginView.h \
    Components/QScintilla/scintilla/src/Partitioning.h \
    Components/QScintilla/scintilla/src/PerLine.h \
    Components/QScintilla/scintilla/src/Position.h \
    Components/QScintilla/scintilla/src/PositionCache.h \
    Components/QScintilla/scintilla/src/RESearch.h \
    Components/QScintilla/scintilla/src/RunStyles.h \
    Components/QScintilla/scintilla/src/ScintillaBase.h \
    Components/QScintilla/scintilla/src/Selection.h \
    Components/QScintilla/scintilla/src/SparseVector.h \
    Components/QScintilla/scintilla/src/SplitVector.h \
    Components/QScintilla/scintilla/src/Style.h \
    Components/QScintilla/scintilla/src/UniConversion.h \
    Components/QScintilla/scintilla/src/UniqueString.h \
    Components/QScintilla/scintilla/src/ViewStyle.h \
    Components/QScintilla/scintilla/src/XPM.h \
    Components/QScintilla/src/ListBoxQt.h \
    Components/QScintilla/src/Qsci/qsciabstractapis.h \
    Components/QScintilla/src/Qsci/qsciapis.h \
    Components/QScintilla/src/Qsci/qscicommand.h \
    Components/QScintilla/src/Qsci/qscicommandset.h \
    Components/QScintilla/src/Qsci/qscidocument.h \
    Components/QScintilla/src/Qsci/qsciglobal.h \
    Components/QScintilla/src/Qsci/qscilexer.h \
    Components/QScintilla/src/Qsci/qscilexercpp.h \
    Components/QScintilla/src/Qsci/qscilexerlinkerscript.h \
    Components/QScintilla/src/Qsci/qscimacro.h \
    Components/QScintilla/src/Qsci/qsciprinter.h \
    Components/QScintilla/src/Qsci/qsciscintilla.h \
    Components/QScintilla/src/Qsci/qsciscintillabase.h \
    Components/QScintilla/src/Qsci/qscistyle.h \
    Components/QScintilla/src/Qsci/qscistyledtext.h \
    Components/QScintilla/src/SciAccessibility.h \
    Components/QScintilla/src/SciClasses.h \
    Components/QScintilla/src/ScintillaQt.h \
    DrcEngine/CDrcManager.h \
    DrcEngine/CDrcViolation.h \
    DrcEngine/DrcCommons.h \
    DrcEngine/EDrcViolationCode.h \
    DrcEngine/EDrcViolationSeverity.h \
    DrcEngine/IDrcRuleBase.h \
    DrcEngine/Rules/CDirectivesDeclaredOnlyOnceRule.h \
    DrcEngine/Rules/CEntryIsDefinedRule.h \
    DrcEngine/Rules/CIncludeRecursionGuard.h \
    DrcEngine/Rules/CIncludedFilesArePresentRule.h \
    DrcEngine/Rules/CLocationCounterCannotGoBackwardsRule.h \
    DrcEngine/Rules/CMemoryRegionDefinedOnlyOnceRule.h \
    DrcEngine/Rules/CNoDuplicateMemoryRegionNameRule.h \
    DrcEngine/Rules/CSectionsDefinedOnlyOnceRule.h \
    DrcEngine/Rules/CSymbolsDefinedInAdvanceRule.h \
    Helpers.h \
    Models/CInputSectionWildcardWord.h \
    Models/CSectionOutputConstraint.h \
    Models/CSectionOutputDataExpression.h \
    Models/CWildcardEntry.h \
    ParsingEngine/SubParsers/CInputSectionFunctionExcludeFileParser.h \
    ParsingEngine/SubParsers/CInputSectionFunctionKeepParser.h \
    ParsingEngine/SubParsers/CInputSectionFunctionSortParser.h \
    ParsingEngine/SubParsers/CSectionOutputCommandParser.h \
    ParsingEngine/SubParsers/SubParserHelpers.h \
    QueryEngine/CLinq.h \
    QueryEngine/QueryCenter.h \
    LinkerScriptManager/CLinkerScriptManager.h \
    LinkerScriptManager/CUndoRedoManager.h \
    Models/CAssignmentProcedureStatement.h \
    Models/CAssignmentStatement.h \
    Models/CExpression.h \
    Models/CExpressionNumber.h \
    Models/CExpressionOperator.h \
    Models/CFunctionCall.h \
    Models/CIncludeCommand.h \
    Models/CInputSectionFunction.h \
    Models/CInputSectionStatement.h \
    Models/CInputSectionTargetSection.h \
    Models/CLinkerScriptContentBase.h \
    Models/CLinkerScriptFile.h \
    Models/CMemoryRegion.h \
    Models/CMemoryStatement.h \
    Models/CMemoryStatementAttribute.h \
    Models/CParameterSeparator.h \
    Models/CPhdrsRegion.h \
    Models/CPhdrsStatement.h \
    Models/CProcedureCall.h \
    Models/CSectionOutputAtLmaRegion.h \
    Models/CSectionOutputCommand.h \
    Models/CSectionOutputCommand.h \
    Models/CSectionOutputFillExpression.h \
    Models/CSectionOutputOverlay.h \
    Models/CSectionOutputPhdr.h \
    Models/CSectionOutputToVmaRegion.h \
    Models/CSectionOutputType.h \
    Models/CSectionsRegion.h \
    Models/CStringEntry.h \
    Models/CSymbol.h \
    Models/CUnrecognizableContent.h \
    Models/CVersionNode.h \
    Models/CVersionScope.h \
    Models/CVersionTag.h \
    Models/CVersionsRegion.h \
    Models/CViolationBase.h \
    Models/Intervention/CActionBase.h \
    Models/Intervention/CAddTextAction.h \
    Models/Intervention/CIntervention.h \
    Models/Intervention/CRemoveTextAction.h \
    Models/Raw/CRawEntry.h \
    Models/Raw/CRawFile.h \
    Models/Raw/RawEntryType.h \
    Models/CSectionOverlayCommand.h \
    ParsingEngine/CMasterParser.h \
    ParsingEngine/CMasterParserException.h \    
    ParsingEngine/CParserViolation.h \
    ParsingEngine/CLexer.h \
    ParsingEngine/CLexerException.h \
    ParsingEngine/EParserViolationCode.h \
    ParsingEngine/SubParsers/CAssignmentParser.h \
    ParsingEngine/SubParsers/CAssignmentProcedureParser.h \
    ParsingEngine/SubParsers/CCommandParser.h \
    ParsingEngine/SubParsers/CExpressionParser.h \
    ParsingEngine/SubParsers/CFunctionParser.h \
    ParsingEngine/SubParsers/CInputSectionStatementParser.h \
    ParsingEngine/SubParsers/CMemoryRegionContentParser.h \
    ParsingEngine/SubParsers/CMemoryRegionParser.h \
    ParsingEngine/SubParsers/CMemoryStatementAttributeParser.h \
    ParsingEngine/SubParsers/CPhdrsRegionContentParser.h \
    ParsingEngine/SubParsers/CPhdrsRegionParser.h \
    ParsingEngine/SubParsers/CScopedRegionParser.h \
    ParsingEngine/SubParsers/CScopedRegionParser.tpp \
    ParsingEngine/SubParsers/CSectionOutputOverlayParser.h \
    ParsingEngine/SubParsers/CSectionOverlayParser.h \
    ParsingEngine/SubParsers/CSubParserBase.h \
    ParsingEngine/SubParsers/CVersionRegionContentParser.h \
    ParsingEngine/SubParsers/CVersionRegionParser.h \
    ParsingEngine/SubParsers/Constants.h \
    ParsingEngine/SubParsers/SubParserType.h \
    ParsingEngine/SubParsers/CSectionsRegionParser.h \
    QueryEngine/QueryCenter.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# For QScintiall, these are needed
DEFINES += SCINTILLA_QT SCI_LEXER INCLUDE_DEPRECATED_FEATURES
INCLUDEPATH += . \
	Components/QScintilla/scintilla/include \
	Components/QScintilla/scintilla/lexlib \
	Components/QScintilla/scintilla/src

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O3
QMAKE_CXXFLAGS_RELEASE += -O0

QMAKE_CFLAGS_RELEASE -= -O1
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE -= -O3
QMAKE_CFLAGS_RELEASE += -O0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

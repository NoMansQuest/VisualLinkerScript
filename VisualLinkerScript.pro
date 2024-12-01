QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# The 'sources' tag is auto-generated from template
SOURCES += \
    Components\InputDialogs\QJumpToLineDialog.cpp \
    Components\InputDialogs\QLabelAssistedInput.cpp \
    Components\InputDialogs\QListAssistedInput.cpp \
    Components\MemoryVisualizer\Composition\CFillExpressionButton.cpp \
    Components\MemoryVisualizer\Composition\CFloorPlan.cpp \
    Components\MemoryVisualizer\Composition\CMemorySection.cpp \
    Components\MemoryVisualizer\Composition\COverlaySectionOutput.cpp \
    Components\MemoryVisualizer\Composition\COverlaySectionStatement.cpp \
    Components\MemoryVisualizer\Composition\COverlayStatement.cpp \
    Components\MemoryVisualizer\Composition\CProgramHeaderButton.cpp \
    Components\MemoryVisualizer\Composition\CSectionOutput.cpp \
    Components\MemoryVisualizer\Composition\CSectionStatement.cpp \
    Components\MemoryVisualizer\CModelTranslator.cpp \
    Components\MemoryVisualizer\QMemoryLayoutRender.cpp \
    Components\MemoryVisualizer\QMemoryVisualizer.cpp \
    Components\QChromeTab\QChromeTabButton.cpp \
    Components\QChromeTab\QChromeTabWidget.cpp \
    Components\QFlatCheckBox\QFlatCheckBox.cpp \
    Components\QLinkerScriptSession\AutoStyling\CAutoStyler.cpp \
    Components\QLinkerScriptSession\CustomComponents\QIssuesTreeView.cpp \
    Components\QLinkerScriptSession\QLinkerScriptSession.cpp \
    Components\QManualSizeGrip\QManualSizeGrip.cpp \
    Components\QScintilla\scintilla\lexers\LexCPP.cpp \
    Components\QScintilla\scintilla\lexers\LexLinkerScript.cpp \
    Components\QScintilla\scintilla\lexlib\Accessor.cpp \
    Components\QScintilla\scintilla\lexlib\CharacterCategory.cpp \
    Components\QScintilla\scintilla\lexlib\CharacterSet.cpp \
    Components\QScintilla\scintilla\lexlib\DefaultLexer.cpp \
    Components\QScintilla\scintilla\lexlib\LexerBase.cpp \
    Components\QScintilla\scintilla\lexlib\LexerModule.cpp \
    Components\QScintilla\scintilla\lexlib\LexerNoExceptions.cpp \
    Components\QScintilla\scintilla\lexlib\LexerSimple.cpp \
    Components\QScintilla\scintilla\lexlib\PropSetSimple.cpp \
    Components\QScintilla\scintilla\lexlib\StyleContext.cpp \
    Components\QScintilla\scintilla\lexlib\WordList.cpp \
    Components\QScintilla\scintilla\src\AutoComplete.cpp \
    Components\QScintilla\scintilla\src\CallTip.cpp \
    Components\QScintilla\scintilla\src\CaseConvert.cpp \
    Components\QScintilla\scintilla\src\CaseFolder.cpp \
    Components\QScintilla\scintilla\src\Catalogue.cpp \
    Components\QScintilla\scintilla\src\CellBuffer.cpp \
    Components\QScintilla\scintilla\src\CharClassify.cpp \
    Components\QScintilla\scintilla\src\ContractionState.cpp \
    Components\QScintilla\scintilla\src\DBCS.cpp \
    Components\QScintilla\scintilla\src\Decoration.cpp \
    Components\QScintilla\scintilla\src\Document.cpp \
    Components\QScintilla\scintilla\src\EditModel.cpp \
    Components\QScintilla\scintilla\src\Editor.cpp \
    Components\QScintilla\scintilla\src\EditView.cpp \
    Components\QScintilla\scintilla\src\ExternalLexer.cpp \
    Components\QScintilla\scintilla\src\Indicator.cpp \
    Components\QScintilla\scintilla\src\KeyMap.cpp \
    Components\QScintilla\scintilla\src\LineMarker.cpp \
    Components\QScintilla\scintilla\src\MarginView.cpp \
    Components\QScintilla\scintilla\src\PerLine.cpp \
    Components\QScintilla\scintilla\src\PositionCache.cpp \
    Components\QScintilla\scintilla\src\RESearch.cpp \
    Components\QScintilla\scintilla\src\RunStyles.cpp \
    Components\QScintilla\scintilla\src\ScintillaBase.cpp \
    Components\QScintilla\scintilla\src\Selection.cpp \
    Components\QScintilla\scintilla\src\Style.cpp \
    Components\QScintilla\scintilla\src\UniConversion.cpp \
    Components\QScintilla\scintilla\src\ViewStyle.cpp \
    Components\QScintilla\scintilla\src\XPM.cpp \
    Components\QScintilla\src\InputMethod.cpp \
    Components\QScintilla\src\ListBoxQt.cpp \
    Components\QScintilla\src\MacPasteboardMime.cpp \
    Components\QScintilla\src\PlatQt.cpp \
    Components\QScintilla\src\qsciabstractapis.cpp \
    Components\QScintilla\src\qsciapis.cpp \
    Components\QScintilla\src\qscicommand.cpp \
    Components\QScintilla\src\qscicommandset.cpp \
    Components\QScintilla\src\qscidocument.cpp \
    Components\QScintilla\src\qscilexer.cpp \
    Components\QScintilla\src\qscilexercpp.cpp \
    Components\QScintilla\src\qscilexerlinkerscript.cpp \
    Components\QScintilla\src\qscimacro.cpp \
    Components\QScintilla\src\qsciprinter.cpp \
    Components\QScintilla\src\qsciscintilla.cpp \
    Components\QScintilla\src\qsciscintillabase.cpp \
    Components\QScintilla\src\qscistyle.cpp \
    Components\QScintilla\src\qscistyledtext.cpp \
    Components\QScintilla\src\SciAccessibility.cpp \
    Components\QScintilla\src\SciClasses.cpp \
    Components\QScintilla\src\ScintillaQt.cpp \
    Components\QScintilla\ComponentHelpers.cpp \
    Components\QScintilla\VlsScintilla.cpp \
    Components\QSearchPopup\QSearchPopup.cpp \
    DrcEngine\Rules\CDirectivesDeclaredOnlyOnceRule.cpp \
    DrcEngine\Rules\CEntryIsDefinedRule.cpp \
    DrcEngine\Rules\CIncludedFilesArePresentRule.cpp \
    DrcEngine\Rules\CIncludeRecursionGuard.cpp \
    DrcEngine\Rules\CLocationCounterCannotGoBackwardsRule.cpp \
    DrcEngine\Rules\CMemoryRegionDefinedOnlyOnceRule.cpp \
    DrcEngine\Rules\CNoDuplicateMemoryRegionNameRule.cpp \
    DrcEngine\Rules\CSectionsDefinedOnlyOnceRule.cpp \
    DrcEngine\Rules\CSymbolsDefinedInAdvanceRule.cpp \
    DrcEngine\CDrcManager.cpp \
    DrcEngine\EDrcViolationCode.cpp \
    LinkerScriptManager\QLinkerScriptManager.cpp \
    MD5\MD5.cpp \
    Models\CAssignmentProcedureStatement.cpp \
    Models\CAssignmentStatement.cpp \
    Models\CExpression.cpp \
    Models\CFunctionCall.cpp \
    Models\CInputSectionFunction.cpp \
    Models\CInputSectionStatement.cpp \
    Models\CLinkerScriptFile.cpp \
    Models\CMemoryRegion.cpp \
    Models\CMemoryStatement.cpp \
    Models\CParsedContentBase.cpp \
    Models\CPhdrsRegion.cpp \
    Models\CPhdrsStatement.cpp \
    Models\CProcedureCall.cpp \
    Models\CSectionOutputCommand.cpp \
    Models\CSectionOutputOverlay.cpp \
    Models\CSectionOverlayCommand.cpp \
    Models\CSectionsRegion.cpp \
    Models\CVersionScope.cpp \
    ParsingEngine\SubParsers\CAssignmentParser.cpp \
    ParsingEngine\SubParsers\CAssignmentProcedureParser.cpp \
    ParsingEngine\SubParsers\CExpressionParser.cpp \
    ParsingEngine\SubParsers\CFunctionParser.cpp \
    ParsingEngine\SubParsers\CInputSectionFunctionExcludeFileParser.cpp \
    ParsingEngine\SubParsers\CInputSectionFunctionKeepParser.cpp \
    ParsingEngine\SubParsers\CInputSectionFunctionSortParser.cpp \
    ParsingEngine\SubParsers\CInputSectionStatementParser.cpp \
    ParsingEngine\SubParsers\CMemoryRegionContentParser.cpp \
    ParsingEngine\SubParsers\CMemoryStatementAttributeParser.cpp \
    ParsingEngine\SubParsers\Constants.cpp \
    ParsingEngine\SubParsers\CPhdrsRegionContentParser.cpp \
    ParsingEngine\SubParsers\CSectionOutputCommandParser.cpp \
    ParsingEngine\SubParsers\CSectionOverlayParser.cpp \
    ParsingEngine\SubParsers\CSectionsRegionParser.cpp \
    ParsingEngine\SubParsers\CSubParserBase.cpp \
    ParsingEngine\SubParsers\CVersionRegionContentParser.cpp \
    ParsingEngine\SubParsers\SubParserHelpers.cpp \
    ParsingEngine\CLinkerScriptLexer.cpp \
    ParsingEngine\CLinkerScriptParser.cpp \
    ParsingEngine\ELexerViolationCode.cpp \
    ParsingEngine\EParserViolationCode.cpp \
    Widgets\QLinkerScriptPanel.cpp \
    Helpers.cpp \
    main.cpp \
    MainWindow.cpp 

# The 'headers' tag is auto-generated from template
HEADERS += \
    Components\InputDialogs\QJumpToLineDialog.h \
    Components\InputDialogs\QLabelAssistedInput.h \
    Components\InputDialogs\QListAssistedInput.h \
    Components\MemoryVisualizer\Composition\CAddressedRegion.h \
    Components\MemoryVisualizer\Composition\CFillExpressionButton.h \
    Components\MemoryVisualizer\Composition\CFloorPlan.h \
    Components\MemoryVisualizer\Composition\CInteractiveObject.h \
    Components\MemoryVisualizer\Composition\CMemorySection.h \
    Components\MemoryVisualizer\Composition\CModelMappedObject.h \
    Components\MemoryVisualizer\Composition\COverlaySectionOutput.h \
    Components\MemoryVisualizer\Composition\COverlaySectionStatement.h \
    Components\MemoryVisualizer\Composition\COverlayStatement.h \
    Components\MemoryVisualizer\Composition\CProgramHeaderButton.h \
    Components\MemoryVisualizer\Composition\CSectionDefinitionBase.h \
    Components\MemoryVisualizer\Composition\CSectionOutput.h \
    Components\MemoryVisualizer\Composition\CSectionStatement.h \
    Components\MemoryVisualizer\Composition\CTooltip.h \
    Components\MemoryVisualizer\Composition\EObjectState.h \
    Components\MemoryVisualizer\Composition\SLineF.h \
    Components\MemoryVisualizer\Composition\SMetricPointF.h \
    Components\MemoryVisualizer\Composition\SMetricRectangleF.h \
    Components\MemoryVisualizer\Composition\SMetricSizeF.h \
    Components\MemoryVisualizer\Composition\SModelSelectionRange.h \
    Components\MemoryVisualizer\Composition\SPixelSizeF.h \
    Components\MemoryVisualizer\CModelTranslator.h \
    Components\MemoryVisualizer\EInsertObjectType.h \
    Components\MemoryVisualizer\QMemoryLayoutRender.h \
    Components\MemoryVisualizer\QMemoryVisualizer.h \
    Components\MemoryVisualizer\SBackendResponse.h \
    Components\QChromeTab\QChromeTabButton.h \
    Components\QChromeTab\QChromeTabWidget.h \
    Components\QFlatCheckBox\QFlatCheckBox.h \
    Components\QLinkerScriptSession\AutoStyling\CAutoStyler.h \
    Components\QLinkerScriptSession\CustomComponents\QIssuesTreeView.h \
    Components\QLinkerScriptSession\EditorAction\EEditorActionType.h \
    Components\QLinkerScriptSession\EditorAction\SEditorActionBase.h \
    Components\QLinkerScriptSession\EditorAction\SEditorAddContent.h \
    Components\QLinkerScriptSession\EditorAction\SEditorAddEmptyLine.h \
    Components\QLinkerScriptSession\EditorAction\SEditorRemoveContent.h \
    Components\QLinkerScriptSession\EditorAction\SEditorRemoveLine.h \
    Components\QLinkerScriptSession\EditorAction\SEditorReplaceContent.h \
    Components\QLinkerScriptSession\EditorAction\SEditorSelectText.h \
    Components\QLinkerScriptSession\EditorAction\SEditorSetCaretPosition.h \
    Components\QLinkerScriptSession\EditorAction\SEditorSetLineContent.h \
    Components\QLinkerScriptSession\EditorAction\SEditorShowAutoCompletion.h \
    Components\QLinkerScriptSession\QLinkerScriptSession.h \
    Components\QManualSizeGrip\QManualSizeGrip.h \
    Components\QScintilla\scintilla\include\ILexer.h \
    Components\QScintilla\scintilla\include\ILoader.h \
    Components\QScintilla\scintilla\include\Platform.h \
    Components\QScintilla\scintilla\include\SciLexer.h \
    Components\QScintilla\scintilla\include\Scintilla.h \
    Components\QScintilla\scintilla\include\ScintillaWidget.h \
    Components\QScintilla\scintilla\include\Sci_Position.h \
    Components\QScintilla\scintilla\lexlib\Accessor.h \
    Components\QScintilla\scintilla\lexlib\CharacterCategory.h \
    Components\QScintilla\scintilla\lexlib\CharacterSet.h \
    Components\QScintilla\scintilla\lexlib\DefaultLexer.h \
    Components\QScintilla\scintilla\lexlib\LexAccessor.h \
    Components\QScintilla\scintilla\lexlib\LexerBase.h \
    Components\QScintilla\scintilla\lexlib\LexerModule.h \
    Components\QScintilla\scintilla\lexlib\LexerNoExceptions.h \
    Components\QScintilla\scintilla\lexlib\LexerSimple.h \
    Components\QScintilla\scintilla\lexlib\OptionSet.h \
    Components\QScintilla\scintilla\lexlib\PropSetSimple.h \
    Components\QScintilla\scintilla\lexlib\SparseState.h \
    Components\QScintilla\scintilla\lexlib\StringCopy.h \
    Components\QScintilla\scintilla\lexlib\StyleContext.h \
    Components\QScintilla\scintilla\lexlib\SubStyles.h \
    Components\QScintilla\scintilla\lexlib\WordList.h \
    Components\QScintilla\scintilla\src\AutoComplete.h \
    Components\QScintilla\scintilla\src\CallTip.h \
    Components\QScintilla\scintilla\src\CaseConvert.h \
    Components\QScintilla\scintilla\src\CaseFolder.h \
    Components\QScintilla\scintilla\src\Catalogue.h \
    Components\QScintilla\scintilla\src\CellBuffer.h \
    Components\QScintilla\scintilla\src\CharClassify.h \
    Components\QScintilla\scintilla\src\ContractionState.h \
    Components\QScintilla\scintilla\src\DBCS.h \
    Components\QScintilla\scintilla\src\Decoration.h \
    Components\QScintilla\scintilla\src\Document.h \
    Components\QScintilla\scintilla\src\EditModel.h \
    Components\QScintilla\scintilla\src\Editor.h \
    Components\QScintilla\scintilla\src\EditView.h \
    Components\QScintilla\scintilla\src\ElapsedPeriod.h \
    Components\QScintilla\scintilla\src\ExternalLexer.h \
    Components\QScintilla\scintilla\src\FontQuality.h \
    Components\QScintilla\scintilla\src\Indicator.h \
    Components\QScintilla\scintilla\src\IntegerRectangle.h \
    Components\QScintilla\scintilla\src\KeyMap.h \
    Components\QScintilla\scintilla\src\LineMarker.h \
    Components\QScintilla\scintilla\src\MarginView.h \
    Components\QScintilla\scintilla\src\Partitioning.h \
    Components\QScintilla\scintilla\src\PerLine.h \
    Components\QScintilla\scintilla\src\Position.h \
    Components\QScintilla\scintilla\src\PositionCache.h \
    Components\QScintilla\scintilla\src\RESearch.h \
    Components\QScintilla\scintilla\src\RunStyles.h \
    Components\QScintilla\scintilla\src\ScintillaBase.h \
    Components\QScintilla\scintilla\src\Selection.h \
    Components\QScintilla\scintilla\src\SparseVector.h \
    Components\QScintilla\scintilla\src\SplitVector.h \
    Components\QScintilla\scintilla\src\Style.h \
    Components\QScintilla\scintilla\src\UniConversion.h \
    Components\QScintilla\scintilla\src\UniqueString.h \
    Components\QScintilla\scintilla\src\ViewStyle.h \
    Components\QScintilla\scintilla\src\XPM.h \
    Components\QScintilla\src\Qsci\qsciabstractapis.h \
    Components\QScintilla\src\Qsci\qsciapis.h \
    Components\QScintilla\src\Qsci\qscicommand.h \
    Components\QScintilla\src\Qsci\qscicommandset.h \
    Components\QScintilla\src\Qsci\qscidocument.h \
    Components\QScintilla\src\Qsci\qsciglobal.h \
    Components\QScintilla\src\Qsci\qscilexer.h \
    Components\QScintilla\src\Qsci\qscilexercpp.h \
    Components\QScintilla\src\Qsci\qscilexerlinkerscript.h \
    Components\QScintilla\src\Qsci\qscimacro.h \
    Components\QScintilla\src\Qsci\qsciprinter.h \
    Components\QScintilla\src\Qsci\qsciscintilla.h \
    Components\QScintilla\src\Qsci\qsciscintillabase.h \
    Components\QScintilla\src\Qsci\qscistyle.h \
    Components\QScintilla\src\Qsci\qscistyledtext.h \
    Components\QScintilla\src\ListBoxQt.h \
    Components\QScintilla\src\SciAccessibility.h \
    Components\QScintilla\src\SciClasses.h \
    Components\QScintilla\src\ScintillaQt.h \
    Components\QScintilla\ComponentHelpers.h \
    Components\QScintilla\VlsScintilla.h \
    Components\QSearchPopup\QSearchPopup.h \
    Components\QSearchPopup\SearchPerimeterTypeEnum.h \
    Components\QSearchPopup\SearchReplaceRequestTypeEnum.h \
    DrcEngine\Rules\CDirectivesDeclaredOnlyOnceRule.h \
    DrcEngine\Rules\CEntryIsDefinedRule.h \
    DrcEngine\Rules\CIncludedFilesArePresentRule.h \
    DrcEngine\Rules\CIncludeRecursionGuard.h \
    DrcEngine\Rules\CLocationCounterCannotGoBackwardsRule.h \
    DrcEngine\Rules\CMemoryRegionDefinedOnlyOnceRule.h \
    DrcEngine\Rules\CNoDuplicateMemoryRegionNameRule.h \
    DrcEngine\Rules\CSectionsDefinedOnlyOnceRule.h \
    DrcEngine\Rules\CSymbolsDefinedInAdvanceRule.h \
    DrcEngine\CCorrectiveAction.h \
    DrcEngine\CDrcManager.h \
    DrcEngine\CDrcViolation.h \
    DrcEngine\DrcCommons.h \
    DrcEngine\EDrcViolationCode.h \
    DrcEngine\IDrcRuleBase.h \
    LinkerScriptManager\QLinkerScriptManager.h \
    MD5\MD5.h \
    MD5\MD5_loc.h \
    Messaging\General\CNotifyUserEvent.h \
    Messaging\LinkerScriptManager\CFileWasClosedEvent.h \
    Messaging\LinkerScriptManager\CFileWasOpenedEvent.h \
    Messaging\UserInitiated\CCloseFileRequest.h \
    Messaging\UserInitiated\CFindReplaceRequest.h \
    Messaging\UserInitiated\CFindRequest.h \
    Messaging\UserInitiated\CLoadMappingFileRequest.h \
    Messaging\UserInitiated\COpenFileRequest.h \
    Messaging\UserInitiated\CRedoRequest.h \
    Messaging\UserInitiated\CSaveFileRequest.h \
    Messaging\UserInitiated\CShowAboutRequest.h \
    Messaging\UserInitiated\CUndoRequest.h \
    Messaging\CEventAggregator.h \
    Messaging\CPubSubEvent.h \
    Models\Raw\CRawEntry.h \
    Models\Raw\CRawFile.h \
    Models\Raw\RawEntryType.h \
    Models\CAssignmentProcedureStatement.h \
    Models\CAssignmentStatement.h \
    Models\CComment.h \
    Models\CExpression.h \
    Models\CExpressionNumber.h \
    Models\CExpressionOperator.h \
    Models\CFunctionCall.h \
    Models\CIncludeCommand.h \
    Models\CIndentationInfo.h \
    Models\CInputSectionFunction.h \
    Models\CInputSectionStatement.h \
    Models\CInputSectionTargetSection.h \
    Models\CInputSectionWildcardWord.h \
    Models\CLinkerScriptFile.h \
    Models\CMemoryRegion.h \
    Models\CMemoryStatement.h \
    Models\CMemoryStatementAttribute.h \
    Models\CParameterSeparator.h \
    Models\CParsedContentBase.h \
    Models\CPhdrsRegion.h \
    Models\CPhdrsStatement.h \
    Models\CProcedureCall.h \
    Models\CSectionOutputAtLmaRegion.h \
    Models\CSectionOutputCommand.h \
    Models\CSectionOutputConstraint.h \
    Models\CSectionOutputDataExpression.h \
    Models\CSectionOutputFillExpression.h \
    Models\CSectionOutputOverlay.h \
    Models\CSectionOutputPhdr.h \
    Models\CSectionOutputToVmaRegion.h \
    Models\CSectionOutputType.h \
    Models\CSectionOverlayCommand.h \
    Models\CSectionsRegion.h \
    Models\CStringEntry.h \
    Models\CSymbol.h \
    Models\CUnrecognizableContent.h \
    Models\CVersionNode.h \
    Models\CVersionScope.h \
    Models\CVersionsRegion.h \
    Models\CVersionTag.h \
    Models\CViolationBase.h \
    Models\CWildcardEntry.h \
    Models\ESeverityCode.h \
    ParsingEngine\SubParsers\CAssignmentParser.h \
    ParsingEngine\SubParsers\CAssignmentProcedureParser.h \
    ParsingEngine\SubParsers\CCommandParser.h \
    ParsingEngine\SubParsers\CExpressionParser.h \
    ParsingEngine\SubParsers\CFunctionParser.h \
    ParsingEngine\SubParsers\CInputSectionFunctionExcludeFileParser.h \
    ParsingEngine\SubParsers\CInputSectionFunctionKeepParser.h \
    ParsingEngine\SubParsers\CInputSectionFunctionSortParser.h \
    ParsingEngine\SubParsers\CInputSectionStatementParser.h \
    ParsingEngine\SubParsers\CMemoryRegionContentParser.h \
    ParsingEngine\SubParsers\CMemoryRegionParser.h \
    ParsingEngine\SubParsers\CMemoryStatementAttributeParser.h \
    ParsingEngine\SubParsers\Constants.h \
    ParsingEngine\SubParsers\CPhdrsRegionContentParser.h \
    ParsingEngine\SubParsers\CPhdrsRegionParser.h \
    ParsingEngine\SubParsers\CScopedRegionParser.h \
    ParsingEngine\SubParsers\CScopedRegionParser.tpp \
    ParsingEngine\SubParsers\CSectionOutputCommandParser.h \
    ParsingEngine\SubParsers\CSectionOutputOverlayParser.h \
    ParsingEngine\SubParsers\CSectionOverlayParser.h \
    ParsingEngine\SubParsers\CSectionsRegionParser.h \
    ParsingEngine\SubParsers\CSubParserBase.h \
    ParsingEngine\SubParsers\CVersionRegionContentParser.h \
    ParsingEngine\SubParsers\CVersionRegionParser.h \
    ParsingEngine\SubParsers\SubParserHelpers.h \
    ParsingEngine\SubParsers\SubParserType.h \
    ParsingEngine\CLexerException.h \
    ParsingEngine\CLexerViolation.h \
    ParsingEngine\CLinkerScriptLexer.h \
    ParsingEngine\CLinkerScriptParser.h \
    ParsingEngine\CMasterParserException.h \
    ParsingEngine\CParserViolation.h \
    ParsingEngine\ELexerViolationCode.h \
    ParsingEngine\EParserViolationCode.h \
    QueryEngine\CLinq.h \
    QueryEngine\QueryCenter.h \
    QueryEngine\QueryCenter.tpp \
    Widgets\QLinkerScriptPanel.h \
    Helpers.h \
    MainWindow.h \
    ui_MainWindow.h 

FORMS += \
    MainWindow.ui

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

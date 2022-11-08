QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
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
    Components/QScintilla/src/qscilexer.cpp \    \
    Components/QScintilla/src/qscilexercpp.cpp \
    Components/QScintilla/src/qscilexerlinkerscript.cpp \
    Components/QScintilla/src/qscimacro.cpp \
    Components/QScintilla/src/qsciprinter.cpp \
    Components/QScintilla/src/qsciscintilla.cpp \
    Components/QScintilla/src/qsciscintillabase.cpp \
    Components/QScintilla/src/qscistyle.cpp \
    Components/QScintilla/src/qscistyledtext.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
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
    Components/QScintilla/src/Qsci/qscilexer.h \     \
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
    mainwindow.h

FORMS += \
    mainwindow.ui

# For QScintiall, these are needed
DEFINES += SCINTILLA_QT SCI_LEXER INCLUDE_DEPRECATED_FEATURES
INCLUDEPATH += . \
	Components/QScintilla/scintilla/include \
	Components/QScintilla/scintilla/lexlib \
	Components/QScintilla/scintilla/src

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc

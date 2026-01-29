// Scintilla source code edit control
/** @file LexerSimple.cxx
 ** A simple lexer with no state.
 **/
// Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <cstdlib>
#include <cassert>

#include <string>

#include "Components/QScintilla/scintilla/include/ILexer.h"
#include "Components/QScintilla/scintilla/include/Scintilla.h"
#include "Components/QScintilla/scintilla/include/SciLexer.h"

#include "Components/QScintilla/scintilla/lexlib/PropSetSimple.h"
#include "Components/QScintilla/scintilla/lexlib/WordList.h"
#include "Components/QScintilla/scintilla/lexlib/LexAccessor.h"
#include "Components/QScintilla/scintilla/lexlib/Accessor.h"
#include "Components/QScintilla/scintilla/lexlib/LexerModule.h"
#include "Components/QScintilla/scintilla/lexlib/LexerBase.h"
#include "Components/QScintilla/scintilla/lexlib/LexerSimple.h"

using namespace Scintilla;

LexerSimple::LexerSimple(const LexerModule *module_) :
	LexerBase(module_->LexClasses(), module_->NamedStyles()),
	module(module_) {
	for (int wl = 0; wl < module->GetNumWordLists(); wl++) {
		if (!wordLists.empty())
			wordLists += "\n";
		wordLists += module->GetWordListDescription(wl);
	}
}

const char * SCI_METHOD LexerSimple::DescribeWordListSets() {
	return wordLists.c_str();
}

void SCI_METHOD LexerSimple::Lex(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess) {
	Accessor astyler(pAccess, &props);
	module->Lex(startPos, lengthDoc, initStyle, keyWordLists, astyler);
	astyler.Flush();
}

void SCI_METHOD LexerSimple::Fold(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess) {
	if (props.GetInt("fold")) {
		Accessor astyler(pAccess, &props);
		module->Fold(startPos, lengthDoc, initStyle, keyWordLists, astyler);
		astyler.Flush();
	}
}

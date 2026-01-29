// Scintilla source code edit control
/** @file LexerNoExceptions.cxx
 ** A simple lexer with no state which does not throw exceptions so can be used in an external lexer.
 **/
// Copyright 1998-2010 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <cstdlib>
#include <cassert>

#include "Components/QScintilla/scintilla/include/ILexer.h"
#include "Components/QScintilla/scintilla/include/Scintilla.h"
#include "Components/QScintilla/scintilla/include/SciLexer.h"

#include "Components/QScintilla/scintilla/lexlib/PropSetSimple.h"
#include "Components/QScintilla/scintilla/lexlib/WordList.h"
#include "Components/QScintilla/scintilla/lexlib/LexAccessor.h"
#include "Components/QScintilla/scintilla/lexlib/Accessor.h"
#include "Components/QScintilla/scintilla/lexlib/LexerModule.h"
#include "Components/QScintilla/scintilla/lexlib/LexerBase.h"
#include "Components/QScintilla/scintilla/lexlib/LexerNoExceptions.h"

using namespace Scintilla;

Sci_Position SCI_METHOD LexerNoExceptions::PropertySet(const char *key, const char *val) {
	try {
		return LexerBase::PropertySet(key, val);
	} catch (...) {
		// Should not throw into caller as may be compiled with different compiler or options
	}
	return -1;
}

Sci_Position SCI_METHOD LexerNoExceptions::WordListSet(int n, const char *wl) {
	try {
		return LexerBase::WordListSet(n, wl);
	} catch (...) {
		// Should not throw into caller as may be compiled with different compiler or options
	}
	return -1;
}

void SCI_METHOD LexerNoExceptions::Lex(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess) {
	try {
		Accessor astyler(pAccess, &props);
		Lexer(startPos, lengthDoc, initStyle, pAccess, astyler);
		astyler.Flush();
	} catch (...) {
		// Should not throw into caller as may be compiled with different compiler or options
		pAccess->SetErrorStatus(SC_STATUS_FAILURE);
	}
}
void SCI_METHOD LexerNoExceptions::Fold(Sci_PositionU startPos, Sci_Position lengthDoc, int initStyle, IDocument *pAccess) {
	try {
		Accessor astyler(pAccess, &props);
		Folder(startPos, lengthDoc, initStyle, pAccess, astyler);
		astyler.Flush();
	} catch (...) {
		// Should not throw into caller as may be compiled with different compiler or options
		pAccess->SetErrorStatus(SC_STATUS_FAILURE);
	}
}

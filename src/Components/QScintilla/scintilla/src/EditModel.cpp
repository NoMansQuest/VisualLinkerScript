// Scintilla source code edit control
/** @file EditModel.cxx
 ** Defines the editor state that must be visible to EditorView.
 **/
// Copyright 1998-2014 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cmath>

#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>

#include "Components/QScintilla/scintilla/include/Platform.h"

#include "Components/QScintilla/scintilla/include/ILoader.h"
#include "Components/QScintilla/scintilla/include/ILexer.h"
#include "Components/QScintilla/scintilla/include/Scintilla.h"

#include "Components/QScintilla/scintilla/src/Position.h"
#include "Components/QScintilla/scintilla/src/UniqueString.h"
#include "Components/QScintilla/scintilla/src/SplitVector.h"
#include "Components/QScintilla/scintilla/src/Partitioning.h"
#include "Components/QScintilla/scintilla/src/RunStyles.h"
#include "Components/QScintilla/scintilla/src/ContractionState.h"
#include "Components/QScintilla/scintilla/src/CellBuffer.h"
#include "Components/QScintilla/scintilla/src/KeyMap.h"
#include "Components/QScintilla/scintilla/src/Indicator.h"
#include "Components/QScintilla/scintilla/src/LineMarker.h"
#include "Components/QScintilla/scintilla/src/Style.h"
#include "Components/QScintilla/scintilla/src/ViewStyle.h"
#include "Components/QScintilla/scintilla/src/CharClassify.h"
#include "Components/QScintilla/scintilla/src/Decoration.h"
#include "Components/QScintilla/scintilla/src/CaseFolder.h"
#include "Components/QScintilla/scintilla/src/Document.h"
#include "Components/QScintilla/scintilla/src/UniConversion.h"
#include "Components/QScintilla/scintilla/src/Selection.h"
#include "Components/QScintilla/scintilla/src/PositionCache.h"
#include "Components/QScintilla/scintilla/src/EditModel.h"

using namespace Scintilla;

Caret::Caret() :
	active(false), on(false), period(500) {}

EditModel::EditModel() : braces{} {
	inOverstrike = false;
	xOffset = 0;
	trackLineWidth = false;
	posDrag = SelectionPosition(Sci::invalidPosition);
	braces[0] = Sci::invalidPosition;
	braces[1] = Sci::invalidPosition;
	bracesMatchStyle = STYLE_BRACEBAD;
	highlightGuideColumn = 0;
	primarySelection = true;
	imeInteraction = imeWindowed;
	foldFlags = 0;
	foldDisplayTextStyle = SC_FOLDDISPLAYTEXT_HIDDEN;
	hotspot = Range(Sci::invalidPosition);
	hoverIndicatorPos = Sci::invalidPosition;
	wrapWidth = LineLayout::wrapWidthInfinite;
	pdoc = new Document(SC_DOCUMENTOPTION_DEFAULT);
	pdoc->AddRef();
	pcs = ContractionStateCreate(pdoc->IsLarge());
}

EditModel::~EditModel() {
	pdoc->Release();
	pdoc = nullptr;
}

#include "HLogView.h"
#include "Colors.h"

/***********************************************************
 * Constructor
 ***********************************************************/
HLogView::HLogView(BRect rect,
				   const char* name,
				   int32 resize,
				   int32 flags)
	: CTextView(rect, name, resize, flags) {
	SetFontAndColor(be_fixed_font, B_FONT_ALL, &LightGreen);

	SetWordWrap(false);
	SetViewColor(Black);
	MakeEditable(false);
}

/***********************************************************
 * Destructor
 ***********************************************************/
HLogView::~HLogView() {
}

/***********************************************************
 * ScrollToEnd
 ***********************************************************/
void
HLogView::ScrollToEnd() {
	Select(TextLength(), TextLength());
	ScrollToSelection();
}
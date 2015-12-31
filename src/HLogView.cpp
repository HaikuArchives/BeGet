#include "HLogView.h"

const rgb_color LightGreen =			{90, 240,90,	255};

/***********************************************************
 * Constructor
 ***********************************************************/
HLogView::HLogView(BRect rect,
				   const char* name,
				   int32 resize,
				   int32 flags)
	: BTextView(rect, name, resize, flags) {
	SetFontAndColor(be_fixed_font, B_FONT_ALL, &LightGreen);

	SetWordWrap(false);
	SetViewColor(0,0,0);
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

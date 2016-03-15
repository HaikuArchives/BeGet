#include "HLogView.h"

const rgb_color LightGreen =			{90, 240,90,	255};

/***********************************************************
 * Constructor
 ***********************************************************/
HLogView::HLogView(const char* name,
				   int32 flags)
	: BTextView(name, flags) {
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

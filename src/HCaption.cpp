#include <stdio.h>
#include <Window.h>
#include <Autolock.h>
#include <String.h>

#include "HApp.h"
#include "HCaption.h"

/***********************************************************
 * Constructor.
 ***********************************************************/
HCaption::HCaption(BRect rect, const char* name, HListView* target)
	: BView(rect, name, B_FOLLOW_ALL_SIDES, B_WILL_DRAW | B_PULSE_NEEDED)
	, fTarget(target)
	, fOld(-1) {
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BRect rect1 = rect;
	rect1.OffsetTo(B_ORIGIN);
	//
	rect1.top += 1;
	//rect1.bottom -=1;
	rect1.right = rect.right - 3;
	rect1.left = rect.left + 1;
	BString title = "";
	title << "0" << " " << _("items");
	view = new BStringView(rect1, "", title.String(), B_FOLLOW_ALL);
	view->SetAlignment(B_ALIGN_RIGHT);
	this->AddChild(view);
	//this->Draw(this->Bounds());
	BFont font;
	view->GetFont(&font);
	font.SetSize(10);
	view->SetFont(&font);
}

/***********************************************************
 * Destructor.
 ***********************************************************/
HCaption::~HCaption() {
}

/***********************************************************
 * Pulse
 *		Set new number if new number is not same as old one.
 ***********************************************************/
void
HCaption::Pulse() {
	if (fTarget != NULL) {
		int32 num = fTarget->CountRows();
		if (num != fOld) {
			fOld = num;
			SetCaption(num);
		}
	}
}

/***********************************************************
 * Set number.
 ***********************************************************/
void
HCaption::SetCaption(int32 num) {
	BAutolock lock(Window());

	BString str  = "";

	if (num == 1)
		str << num << " " << _("item");
	else
		str << num << " " << _("items");
	if (lock.IsLocked()) {
		view->SetText(str.String());
	}
}

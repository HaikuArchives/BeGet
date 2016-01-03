#include <stdio.h>
#include <Window.h>
#include <Autolock.h>
#include <String.h>

#include "HApp.h"
#include "HCaption.h"

/***********************************************************
 * Constructor.
 ***********************************************************/
HCaption::HCaption(const char* name, HListView* target)
	: BStringView(name,"", B_WILL_DRAW | B_PULSE_NEEDED)
	, fTarget(target)
	, fOld(-1) {
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BString title = "";
	title << "0" << " " << _("items");
	SetText(title.String());
	SetAlignment(B_ALIGN_RIGHT);
	//this->Draw(this->Bounds());
	BFont font;
	GetFont(&font);
	font.SetSize(10);
	SetFont(&font);
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
		SetText(str.String());
	}
}

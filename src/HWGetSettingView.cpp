#include <ClassInfo.h>
#include <TextControl.h>
#include <CheckBox.h>
#include <Button.h>
#include <Roster.h>
#include <Beep.h>
#include <ScrollView.h>
#include <StringView.h>

#include "HPrefs.h"
#include "HWGetSettingView.h"
#include "HApp.h"
#include "CTextView.h"

/***********************************************************
 * Constructor.
 ***********************************************************/
HWGetSettingView::HWGetSettingView(BRect rect)
	: BView(rect, "wgetsetting", B_FOLLOW_ALL, B_WILL_DRAW) {
	InitGUI();
}

/***********************************************************
 * Destructor.
 ***********************************************************/
HWGetSettingView::~HWGetSettingView() {
}

/***********************************************************
 * Set up GUIs.
 ***********************************************************/
void
HWGetSettingView::InitGUI() {
	this->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	BRect rect = Bounds();
	rect.left += 10;
	rect.right -= 10 + B_V_SCROLL_BAR_WIDTH;
	rect.top += 10;
	rect.bottom -= 50;

	fOptionView = new CTextView(rect, "default", B_FOLLOW_ALL, B_WILL_DRAW);

	const char* option;
	((HApp*)be_app)->Prefs()->GetData("option", &option);

	fOptionView->SetText(option);
	fOptionView->SetFont(be_fixed_font);

	BScrollView* scroll = new BScrollView("scroll", fOptionView, B_FOLLOW_ALL, B_WILL_DRAW, false, true);

	rect.top = rect.bottom + 2;
	rect.bottom = Bounds().bottom - 30;
	BString label = _("Default option is");
	label += " \"-c -P /boot/home/Downloads\"";
	BStringView* string = new BStringView(rect, "default"
										  , label.String());
	AddChild(string);

	this->AddChild(scroll);
}

/***********************************************************
 * MessageReceived
 ***********************************************************/
void
HWGetSettingView::MessageReceived(BMessage* message) {
	switch (message->what) {

		default:
			BView::MessageReceived(message);
	}
}

/***********************************************************
 * Option
 ***********************************************************/
const char*
HWGetSettingView::Option() const {
	return fOptionView->Text();
}


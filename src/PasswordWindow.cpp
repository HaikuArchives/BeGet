#include "PasswordWindow.h"

#include "HListItem.h"
#include "PassControl.h"
#include "HApp.h"
#include "HWindow.h"

#include <View.h>
#include <TextControl.h>
#include <Button.h>
#include <ClassInfo.h>


/***********************************************************
 * Constructor
 ***********************************************************/
PasswordWindow::PasswordWindow(BRect frame
							   , const char* name
							   , const char* login_str
							   , const char* pass_str
							   , HListItem* item)
	: BWindow(frame, name, B_TITLED_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL
			  , B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_NOT_CLOSABLE)
	, fListItem(item) {
	BView* view = new BView(Bounds(), "", B_FOLLOW_ALL, B_WILL_DRAW);
	view->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(view);

	BRect rect = Bounds();
	rect.top += 10;
	rect.left += 10;
	rect.right -= 10;
	rect.bottom = rect.top + 25;

	int32 divider = static_cast<int32>(view->StringWidth(_("Password:")) + 2);
	BTextControl* login = new BTextControl(rect, "login", _("Login:"), login_str, new BMessage('LOGI'));
	login->SetDivider(divider);
	view->AddChild(login);

	rect.OffsetBy(0, 30);
	PassControl* pass = new PassControl(rect, "pass", _("Password:"), "", new BMessage('PSAS'));
	pass->SetDivider(divider);
	pass->SetText(pass_str);
	view->AddChild(pass);

	rect.OffsetBy(0, 30);
	rect.left = rect.right - 70;
	BButton* button;
	button = new BButton(rect, "ok", _("OK"), new BMessage('MOKM'));
	//button->SetEnabled(false);
	view->AddChild(button);
	//this->SetDefaultButton(button);

	rect.OffsetBy(-80, 0);
	button = new BButton(rect, "cancel", _("Cancel"), new BMessage(B_QUIT_REQUESTED));
	view->AddChild(button);

	AddShortcut('W', 0, new BMessage(B_QUIT_REQUESTED));

	login->MakeFocus(true);
}

/***********************************************************
 * Destructor
 ***********************************************************/
PasswordWindow::~PasswordWindow() {

}

/***********************************************************
 * MessageReceived
 ***********************************************************/
void
PasswordWindow::MessageReceived(BMessage* message) {
	//message->PrintToStream();
	switch (message->what) {
		case 'MOKM': {
				BMessage msg(M_PASS_MSG);
				PassControl* pass = cast_as(FindView("pass"), PassControl);
				BTextControl* ctrl = cast_as(FindView("login"), BTextControl);

				msg.AddString("login", ctrl->Text());
				msg.AddString("password", pass->actualText());
				msg.AddPointer("pointer", fListItem);

				((HApp*)be_app)->Window()->PostMessage(&msg);
				this->PostMessage(B_QUIT_REQUESTED);
				break;
			}
		case 'LOGI': {
				BTextControl* ctrl = cast_as(FindView("pass"), BTextControl);
				ctrl->MakeFocus(true);
				break;
			}
		case 'PSAS': {
				BButton* button = cast_as(FindView("ok"), BButton);
				button->MakeFocus(true);
				break;
			}
		default:
			BWindow::MessageReceived(message);
	}
}
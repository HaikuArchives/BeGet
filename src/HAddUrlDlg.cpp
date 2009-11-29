#include "HAddUrlDlg.h"
#include "CTextView.h"
#include "HApp.h"
#include "HWindow.h"

#include <Message.h>
#include <ScrollView.h>
#include <Button.h>
#include <String.h>

/***********************************************************
 * Constructor
 ***********************************************************/
HAddUrlDlg::HAddUrlDlg(BRect rect, const char* url)
	: BWindow(rect, _("Add URLs"), B_DOCUMENT_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, B_NOT_ZOOMABLE) {
	InitGUI();
	AddShortcut(B_RETURN, 0, new BMessage(M_OK_MESSAGE));
	AddShortcut('W', 0, new BMessage(B_QUIT_REQUESTED));
	if (url)
		fURLView->Insert(url);
	fURLView->MakeFocus(true);
}

/***********************************************************
 * Destructor
 ***********************************************************/
HAddUrlDlg::~HAddUrlDlg() {

}

/***********************************************************
 * InitGUI
 ***********************************************************/
void
HAddUrlDlg::InitGUI() {
	BView* bg = new BView(Bounds(), "bg", B_FOLLOW_ALL, B_WILL_DRAW);
	bg->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BRect rect = Bounds();
	rect.left += 5;
	rect.top += 5;
	rect.bottom -= 5 + 30 + B_H_SCROLL_BAR_HEIGHT;
	rect.right -= 5 + B_V_SCROLL_BAR_WIDTH;

	fURLView = new CTextView(rect, "url", B_FOLLOW_ALL, B_WILL_DRAW | B_NAVIGABLE);

	BScrollView* scroll = new BScrollView("scroll", fURLView, B_FOLLOW_ALL, B_WILL_DRAW, false, true);
	bg->AddChild(scroll);

	rect.top = rect.bottom + 5 + B_H_SCROLL_BAR_HEIGHT;
	rect.bottom = Bounds().bottom - 5;
	rect.left = rect.right - 80;

	BButton* button = new BButton(rect, "add", _("Add"), new BMessage(M_OK_MESSAGE), B_FOLLOW_BOTTOM | B_FOLLOW_RIGHT);
	bg->AddChild(button);

	AddChild(bg);
}

/***********************************************************
 * MessageReceived
 ***********************************************************/
void
HAddUrlDlg::MessageReceived(BMessage* message) {
	switch (message->what) {
		case M_OK_MESSAGE: {
				BMessage msg(M_REAL_ADD);
				const char* kText = fURLView->Text();
				int32 len = fURLView->TextLength();
				BString line;
				for (int32 i = 0; i < len; i++) {
					if (kText[i] == '\n') {
						if (line.Length() > 0)
							msg.AddString("url", line);
						line = "";
					} else
						line << kText[i];
				}
				if (line.Length() > 0)
					msg.AddString("url", line);
				((HApp*)be_app)->Window()->PostMessage(&msg);
				PostMessage(B_QUIT_REQUESTED);
				break;
			}
		default:
			BWindow::MessageReceived(message);
	}
}
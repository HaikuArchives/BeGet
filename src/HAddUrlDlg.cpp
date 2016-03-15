#include "HAddUrlDlg.h"
#include "HApp.h"
#include "HWindow.h"

#include <Message.h>
#include <ScrollView.h>
#include <Button.h>
#include <String.h>
#include <GroupLayout.h>

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
	BGroupLayout* layout=new BGroupLayout(B_VERTICAL);
	BView* bg = new BView(Bounds(), "bg", B_FOLLOW_ALL, B_WILL_DRAW);
	bg->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	bg->SetLayout(layout);

	fURLView = new BTextView("url", B_WILL_DRAW | B_NAVIGABLE);

	BScrollView* scroll = new BScrollView("scroll", fURLView, B_WILL_DRAW, false, true);
	layout->AddView(scroll,5);

	BButton* button = new BButton("add", _("Add"), new BMessage(M_OK_MESSAGE));
	layout->AddView(button,1);

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

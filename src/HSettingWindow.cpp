#include "HSettingWindow.h"
#include "HApp.h"
#include "HWGetSettingView.h"
#include "HBeGetSettingView.h"
#include "HSoundSettingView.h"
#include "HPrefs.h"
#include <TabView.h>
#include <Button.h>

/***********************************************************
 * Constructor.
 ***********************************************************/
HSettingWindow::HSettingWindow(BRect rect)
	:BWindow(rect,_("Preferences"),B_TITLED_WINDOW_LOOK,
		B_MODAL_APP_WINDOW_FEEL,
		B_NOT_RESIZABLE|B_NOT_ZOOMABLE|B_ASYNCHRONOUS_CONTROLS)
{
	InitGUI();
	this->AddShortcut(B_RETURN,0,new BMessage(M_APPLY_MESSAGE));	
	this->AddShortcut('W',0,new BMessage(B_QUIT_REQUESTED));
}

/***********************************************************
 * Destructor.
 ***********************************************************/
HSettingWindow::~HSettingWindow()
{

}

/***********************************************************
 * MessageReceived
 ***********************************************************/
void
HSettingWindow::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
	case M_APPLY_MESSAGE:
	{
		HPrefs *prefs = ((HApp*)be_app)->Prefs();
		prefs->SetData("auto_launch",begetsetting->Launch());
		prefs->SetData("auto_start",begetsetting->AutoStart());
		prefs->SetData("option",wgetsetting->Option());
		prefs->SetData("auto_delete",begetsetting->AutoDelete());
		prefs->SetData("watch",begetsetting->Watch());
		prefs->SetData("watch_ext",begetsetting->Ext());
		prefs->SetData("watch_confirm",begetsetting->Confirm());
		prefs->SetData("deskbar",begetsetting->Deskbar());
		prefs->SetData("snd_download",soundsetting->DownloadSound());
		prefs->SetData("tracker_rule",begetsetting->TrackerRule());
		prefs->SetData("queue",begetsetting->Queue());
		prefs->SetData("max_transfer",begetsetting->MaxTransfers());
		
		this->PostMessage(B_QUIT_REQUESTED);
		break;
	}
	case M_WATCH_CHANGED:
	case M_DELETE_CHANGED:
	case M_QUEUE_CAHNGED:
	{
		PostMessage(message,begetsetting);
		break;
	}
	case B_REFS_RECEIVED:
	case M_OTHER_MESSAGE:
	case B_CANCEL:
	case M_ITEM_MESSAGE:
	case M_NONE_MESSAGE:
		PostMessage(message,soundsetting);
		break;
	default:
		BWindow::MessageReceived(message);
	}
}

/***********************************************************
 * Set up GUIs.
 ***********************************************************/
void
HSettingWindow::InitGUI()
{
	BRect rect = Bounds();
	rect.bottom -= 35;
	BTabView *tabview = new BTabView(rect,"tabview");
	tabview->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	BTab *tab;
	BRect frame = tabview->Bounds();
//*********** BeGet Setting ******************/
	tab = new BTab();
	tabview->AddTab(begetsetting = new HBeGetSettingView(frame),tab);
	tab->SetLabel(_("BeGet settings"));
//*********** WGet Setting ******************/
	tab = new BTab();
	tabview->AddTab(wgetsetting = new HWGetSettingView(frame),tab);
	tab->SetLabel(_("wget options"));
//*********** Sound Setting ******************/
	tab = new BTab();
	tabview->AddTab(soundsetting = new HSoundSettingView(frame),tab);
	tab->SetLabel(_("Sound"));
	
	AddChild(tabview);

	BRect bgrect = Bounds();
	bgrect.top = bgrect.bottom - 35;
	BView *bgview = new BView(bgrect,"bgview",B_FOLLOW_ALL,B_WILL_DRAW);
	bgview->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	bgrect.OffsetTo(B_ORIGIN);
	bgrect.top += 5;
	bgrect.right -= 10;
	bgrect.left = bgrect.right - 80;
	bgrect.bottom -= 5;

	BButton *button = new BButton(bgrect,"apply",_("Apply"),new BMessage(M_APPLY_MESSAGE));
	bgview->AddChild(button);	
	
	this->AddChild(bgview);
}

/***********************************************************
 * QuitRequested
 ***********************************************************/
bool
HSettingWindow::QuitRequested()
{
	return BWindow::QuitRequested();
}
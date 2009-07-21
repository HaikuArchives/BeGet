#include <ClassInfo.h>
#include <TextControl.h>
#include <CheckBox.h>
#include <Button.h>
#include <Roster.h>
#include <Beep.h>

#include "HPrefs.h"
#include "NumberControl.h"
#include "HBeGetSettingView.h"
#include "HApp.h"

/***********************************************************
 * Constructor.
 ***********************************************************/
HBeGetSettingView::HBeGetSettingView(BRect rect)
	:BView(rect,"wgetsetting",B_FOLLOW_ALL,B_WILL_DRAW)
{
	InitGUI();
}

/***********************************************************
 * Destructor.
 ***********************************************************/
HBeGetSettingView::~HBeGetSettingView()
{
}

/***********************************************************
 * Set up GUIs.
 ***********************************************************/
void
HBeGetSettingView::InitGUI()
{
	this->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	BRect rect = Bounds();
	rect.left += 10;
	rect.right -= 10;
	rect.top += 10;
	rect.bottom = rect.top + 20;
	BCheckBox *checkBox = new BCheckBox(rect,"launch"
		,_("Automatically launch files after they have finished downloading"),NULL);
	bool which;
	((HApp*)be_app)->Prefs()->GetData("auto_launch",&which);
	checkBox->SetValue(which);
	this->AddChild(checkBox);
	
	rect.OffsetBy(0,25);
	checkBox = new BCheckBox(rect,"auto_start"
		,_("Automatically start downloading files when they have added"),NULL);

	((HApp*)be_app)->Prefs()->GetData("auto_start",&which);
	checkBox->SetValue(which);
	this->AddChild(checkBox);
	
	rect.OffsetBy(0,25);
	BRect childRect = rect;
	childRect.left += 30;
	
	checkBox = new BCheckBox(rect,"auto_delete"
		,_("Delete downloaded items after they have finished"),new BMessage(M_DELETE_CHANGED));

	((HApp*)be_app)->Prefs()->GetData("auto_delete",&which);
	checkBox->SetValue(which);	
	this->AddChild(checkBox);
	
	rect.OffsetBy(0,25);
	childRect.OffsetBy(0,25);
	checkBox = new BCheckBox(childRect,"tracker_rule"
		,_("Delete downloaded files when delete its list items"),NULL);
	checkBox->SetEnabled(!which);
	((HApp*)be_app)->Prefs()->GetData("tracker_rule",&which);
	checkBox->SetValue(which);	
	this->AddChild(checkBox);
	
	rect.OffsetBy(0,25);
	childRect.OffsetBy(0,25);
	checkBox = new BCheckBox(rect,"watch"
		,_("Watch URL in the clipboard"),new BMessage(M_WATCH_CHANGED));

	((HApp*)be_app)->Prefs()->GetData("watch",&which);
	checkBox->SetValue(which);	
	this->AddChild(checkBox);
	
	rect.OffsetBy(0,25);
	childRect.OffsetBy(0,25);
	
	const char* ext;
	((HApp*)be_app)->Prefs()->GetData("watch_ext",&ext);
	fExtensions = new BTextControl(childRect,"ext",_("Extensions:"),ext,NULL);
	fExtensions->SetDivider(StringWidth(_("Extensions:"))+2);
	AddChild(fExtensions);
	fExtensions->SetEnabled(which);
	
	rect.OffsetBy(0,25);
	childRect.OffsetBy(0,25);
	
	checkBox = new BCheckBox(childRect,"confirm"
		,_("Confirm when add url from clipboard"),NULL);
	checkBox->SetEnabled(which);
	((HApp*)be_app)->Prefs()->GetData("watch_confirm",&which);
	checkBox->SetValue(which);	
	this->AddChild(checkBox);
	
	rect.OffsetBy(0,25);
	
	checkBox = new BCheckBox(rect,"deskbar"
		,_("Install deskbar replicant"),NULL);
	((HApp*)be_app)->Prefs()->GetData("deskbar",&which);
	checkBox->SetValue(which);	
	this->AddChild(checkBox);
	
	rect.OffsetBy(0,25);
	rect.right = rect.left+StringWidth(_("Queue downloading"))+50;
	checkBox = new BCheckBox(rect,"queue"
		,_("Queue downloading"),new BMessage(M_QUEUE_CAHNGED));
	((HApp*)be_app)->Prefs()->GetData("queue",&which);
	checkBox->SetValue(which);	
	this->AddChild(checkBox);
	
	rect.OffsetBy(rect.Width(),0);
	rect.right = rect.left + StringWidth(_("Max Transfers:"))+50;
	NumberControl *nctrl = new NumberControl(rect,"max_trans",_("Max Transfers:"),5,NULL);
	int32 max;
	((HApp*)be_app)->Prefs()->GetData("max_transfer",&max);
	nctrl->SetValue(max);
	nctrl->SetDivider(StringWidth(_("Max Transfers:"))+5);
	nctrl->SetEnabled(which);
	AddChild(nctrl);
}

/***********************************************************
 * MessageReceived
 ***********************************************************/
void
HBeGetSettingView::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
	case M_WATCH_CHANGED:
	{
		BCheckBox *check = cast_as(FindView("watch"),BCheckBox);
		fExtensions->SetEnabled(check->Value());
		BCheckBox *confirm = cast_as(FindView("confirm"),BCheckBox);
		confirm->SetEnabled(check->Value());
		break;
	}
	case M_QUEUE_CAHNGED:
	{
		BCheckBox *checkbox = cast_as(FindView("queue"),BCheckBox);
		NumberControl *ctrl = cast_as(FindView("max_trans"),NumberControl);
		if(ctrl && checkbox)
			ctrl->SetEnabled(checkbox->Value());
		break;
	}
	case M_DELETE_CHANGED:
	{
		BCheckBox *auto_del = cast_as(FindView("auto_delete"),BCheckBox);
		BCheckBox *rule = cast_as(FindView("tracker_rule"),BCheckBox);
		bool al = auto_del->Value();
		rule->SetEnabled(!al );
		if(al)
			rule->SetValue(!al);
	}
	default:
		BView::MessageReceived(message);
	}
}

/***********************************************************
 * Launch files when finished
 ***********************************************************/
bool
HBeGetSettingView::Launch()
{
	BCheckBox *checkBox = cast_as(FindView("launch"),BCheckBox);
	return checkBox->Value();
}

/***********************************************************
 * Auto start
 ***********************************************************/
bool
HBeGetSettingView::AutoStart()
{
	BCheckBox *checkBox = cast_as(FindView("auto_start"),BCheckBox);
	return checkBox->Value();
}

/***********************************************************
 * Auto delete
 ***********************************************************/
bool
HBeGetSettingView::AutoDelete()
{
	BCheckBox *checkBox = cast_as(FindView("auto_delete"),BCheckBox);
	return checkBox->Value();
}

/***********************************************************
 * Watch
 ***********************************************************/
bool
HBeGetSettingView::Watch()
{
	BCheckBox *checkBox = cast_as(FindView("watch"),BCheckBox);
	return checkBox->Value();
}

/***********************************************************
 * Ext
 ***********************************************************/
const char*
HBeGetSettingView::Ext()
{
	return fExtensions->Text();
}

/***********************************************************
 * Confirm
 ***********************************************************/
bool
HBeGetSettingView::Confirm()
{
	BCheckBox *checkBox = cast_as(FindView("confirm"),BCheckBox);
	return checkBox->Value();
}

/***********************************************************
 * Deskbar
 ***********************************************************/
bool
HBeGetSettingView::Deskbar()
{
	BCheckBox *checkBox = cast_as(FindView("deskbar"),BCheckBox);
	return checkBox->Value();
}

/***********************************************************
 * TrackerRule
 ***********************************************************/
bool
HBeGetSettingView::TrackerRule()
{
	BCheckBox *checkBox = cast_as(FindView("tracker_rule"),BCheckBox);
	return checkBox->Value();
}

/***********************************************************
 * Queue
 ***********************************************************/
bool
HBeGetSettingView::Queue()
{
	BCheckBox *checkBox = cast_as(FindView("queue"),BCheckBox);
	return checkBox->Value();	
}

/***********************************************************
 * MaxTransfer
 ***********************************************************/
int32
HBeGetSettingView::MaxTransfers()
{
	NumberControl *ctrl = cast_as(FindView("max_trans"),NumberControl);
	return ctrl->Value();	
}
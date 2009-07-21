#include "HTaskView.h"
#include <Message.h>
#include <Autolock.h>
#include <Bitmap.h>
#include <PopUpMenu.h>
#include <Window.h>
#include <MenuItem.h>
#include <Entry.h>
#include <Roster.h>
#include <Alert.h>
#include <Messenger.h>
#include <NodeInfo.h>
#include <stdlib.h>

#include "HWindow.h"
#include "MenuUtils.h"
#include "HApp.h"
#include "LocaleUtils.h"

/***********************************************************
 * This is the exported function that will be used by Deskbar
 * to create and add the replicant
 ***********************************************************/
extern "C" _EXPORT BView* instantiate_deskbar_item();


/***********************************************************
 * Constructor.
 ***********************************************************/
HTaskView::HTaskView(BRect frame)
		:BView(frame,"BeGet",B_FOLLOW_NONE,B_WILL_DRAW)
		,fIcon(NULL)
{
	const char* kLabels[] = {"Add New Download","Open Downloads Folder","Quit"};
	for(int32 i = 0;i < 3;i++)
	{
		fLabels[i] = strdup( kLabels[i] );
	}
}

/***********************************************************
 * Deskbar item installing function
 ***********************************************************/
BView* instantiate_deskbar_item(void)
{
	return new HTaskView(BRect(0, 0, 15, 15));	
}

/***********************************************************
 * Constructor for achiving.
 ***********************************************************/
HTaskView::HTaskView(BMessage *message)
	:BView(message)
	,fIcon(NULL)
{
	LocaleUtils utils(APP_SIG);
	const char* kLabels[] = {"Add New Download","Open Downloads Folder","Quit"};
	for(int32 i = 0;i < 3;i++)
	{
		fLabels[i] = strdup( utils.GetText(kLabels[i]) );
	}

}

/***********************************************************
 * Destructor
 ***********************************************************/
HTaskView::~HTaskView()
{
	delete fIcon;
	for(int32 i = 0;i < 3;i++)
		free(fLabels[i]);
}


/***********************************************************
 * Instantiate
 ***********************************************************/
HTaskView*
HTaskView::Instantiate(BMessage *data)
{
	if (!validate_instantiation(data, "HTaskView"))
		return NULL;	

	return new HTaskView(data);
}

/***********************************************************
 * Archive
 ***********************************************************/
status_t
HTaskView::Archive(BMessage *data,bool deep) const
{
	BView::Archive(data, deep);

	data->AddString("add_on",APP_SIG);
	return B_NO_ERROR;
}

/***********************************************************
 * Draw
 ***********************************************************/
void
HTaskView::Draw(BRect /*updateRect*/)
{	
	rgb_color oldColor = HighColor();
	SetHighColor(Parent()->ViewColor());
	FillRect(BRect(0.0,0.0,15.0,15.0));
	SetHighColor(oldColor);
	SetDrawingMode(B_OP_OVER);
	if(fIcon == NULL)
	{
		entry_ref ref;
		if(be_roster->FindApp(APP_SIG,&ref) == B_OK)
		{
			fIcon = new BBitmap(BRect(0.0,0.0,15.0,15.0),B_COLOR_8_BIT,true);
			BNodeInfo::GetTrackerIcon(&ref,fIcon,B_MINI_ICON);
		}
	}
	DrawBitmap(fIcon,BRect(0.0,0.0,15.0,15.0));
	SetDrawingMode(B_OP_COPY);			
	Sync();
}

/***********************************************************
 * MessageReceived
 ***********************************************************/
void
HTaskView::MessageReceived(BMessage *message)
{
	if(message->WasDropped())
		WhenDropped(message);
	else
		BView::MessageReceived(message);
}

/***********************************************************
 * WhenDropped
 ***********************************************************/
void
HTaskView::WhenDropped(BMessage *message)
{
	message->PrintToStream();
	const char* text;
	if( message->FindString("be:url",&text) == B_OK)
	{
		BMessenger messenger(APP_SIG);
		BMessage msg(B_REFS_RECEIVED),reply;
		
		msg.AddString("be:url",text);
		
		messenger.SendMessage(&msg,&reply);
	}
}

/***********************************************************
 * MouseDown
 ***********************************************************/
void
HTaskView::MouseDown(BPoint pos)
{
	entry_ref app;
	BMessage msg(M_SHOW_MSG);
	int32 buttons = 0;
	Window()->CurrentMessage()->FindInt32("buttons", &buttons); 
	
	
  	BPopUpMenu *theMenu = new BPopUpMenu("RIGHT_CLICK",false,false);
  	BFont font(be_plain_font);
  	font.SetSize(10);
  	theMenu->SetFont(&font);
  	
  	MenuUtils utils;
  	utils.AddMenuItem(theMenu,fLabels[0],M_ADD_URL,NULL,NULL,0,0);
  	utils.AddMenuItem(theMenu,fLabels[1],M_OPEN_FOLDER,NULL,NULL,0,0);
	theMenu->AddSeparatorItem();
	//utils.AddMenuItem(theMenu,"About",B_ABOUT_REQUESTED,NULL,NULL,0,0);
  	utils.AddMenuItem(theMenu,fLabels[2],B_QUIT_REQUESTED,NULL,NULL,0,0);
  	
	BRect r ;
   	ConvertToScreen(&pos);
   	r.top = pos.y - 5;
   	r.bottom = pos.y + 5;
   	r.left = pos.x -5;
   	r.right = pos.x +5;
         
	BMenuItem *bItem = theMenu->Go(pos, false,true,r);  
    if(bItem)
    {
    	BMessage*	aMessage = bItem->Message();
		if(aMessage)
		{
			if(be_roster->IsRunning(APP_SIG))
			{
				team_id id = be_roster->TeamFor(APP_SIG);
				BMessenger messenger(APP_SIG,id);
				messenger.SendMessage(aMessage);
			}	
		}
	}
	delete theMenu;
	
	BView::MouseDown(pos);
}
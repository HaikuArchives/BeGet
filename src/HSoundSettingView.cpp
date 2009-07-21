#include <ClassInfo.h>
#include <TextControl.h>
#include <CheckBox.h>
#include <Button.h>
#include <Roster.h>
#include <Beep.h>
#include <ScrollView.h>
#include <StringView.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <Path.h>
#include <NodeInfo.h>
#include <iostream>
#include <PlaySound.h>

#include "HMenuItem.h"
#include "HPrefs.h"
#include "HSoundSettingView.h"
#include "HApp.h"

/***********************************************************
 * Constructor.
 ***********************************************************/
HSoundSettingView::HSoundSettingView(BRect rect)
	:BView(rect,"wgetsetting",B_FOLLOW_ALL,B_WILL_DRAW)
	,fFilePanel(NULL)
{
	InitGUI();
	SetupMenuField();
}

/***********************************************************
 * Destructor.
 ***********************************************************/
HSoundSettingView::~HSoundSettingView()
{
	delete fFilePanel;
}

/***********************************************************
 * Set up GUIs.
 ***********************************************************/
void
HSoundSettingView::InitGUI()
{
	this->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	BRect rect = Bounds();
	rect.left += 10;
	rect.right -= 10 + B_V_SCROLL_BAR_WIDTH;
	rect.top += 10;
	rect.bottom -= 50;
		
	BMenu *menu = new BMenu("file");
	menu->SetRadioMode(true);
	menu->SetLabelFromMarked(true);
	menu->AddSeparatorItem();
	
	BString label = "<";
	label << _("none") << ">";
	menu->AddItem(new BMenuItem(label.String(),new BMessage(M_NONE_MESSAGE)));
	menu->AddItem(new BMenuItem(_("Other…"),new BMessage(M_OTHER_MESSAGE)));	
	
	BMenuField *menuField = new BMenuField(rect
										,"filemenu"
										,_("Download:")
										,menu
										,B_FOLLOW_TOP|B_FOLLOW_LEFT);
	menuField->SetDivider(menuField->StringWidth(_("Download:"))+5);	
	AddChild(menuField);
}

/***********************************************************
 * MessageReceived
 ***********************************************************/
void
HSoundSettingView::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
	case M_OTHER_MESSAGE:
		{
			Window()->SetFeel(B_NORMAL_WINDOW_FEEL);
			if(!fFilePanel)
				fFilePanel = new BFilePanel();
			
			fFilePanel->SetTarget(this);
			fFilePanel->Show();
			
			break;
		}
	case M_ITEM_MESSAGE:
		{
			entry_ref ref;
			if(message->FindRef("refs",&ref)== B_OK)
			{
				::play_sound(&ref,false,false,true);
			}	
		}
	case M_NONE_MESSAGE:
		{
			BMenuField *menufield = cast_as(FindView("filemenu"),BMenuField);
			BMenu *menu = menufield->Menu();
			
			fOldItem = menu->IndexOf(menu->FindMarked());
			break;
		}
	case B_REFS_RECEIVED:
		{
			entry_ref ref;
			if(message->FindRef("refs",&ref) == B_OK)
			{
				BMenuField *menufield = cast_as(FindView("filemenu"),BMenuField);
				BMenu *menu = menufield->Menu();
				// check audio file
				BNode node(&ref);
				BNodeInfo ninfo(&node);
				char type[B_MIME_TYPE_LENGTH+1];
				ninfo.GetType(type);
				BMimeType mtype(type);
				BMimeType superType;
				mtype.GetSupertype(&superType);
				if(strcmp(superType.Type(),"audio") != 0)
				{
					beep();
					(new BAlert("",_("This is not a audio file."),_("OK"),NULL,NULL,
							B_WIDTH_AS_USUAL,B_STOP_ALERT))->Go();
					break;
				}
				// add file item
				BPath path(&ref);
				if(!(menu->FindItem(path.Leaf())))
				{
					BMessage *msg = new BMessage(M_ITEM_MESSAGE);
					msg->AddRef("refs",&ref);
					HMenuItem *item;
					menu->AddItem(item = new HMenuItem(path.Leaf(),msg),0);
					item->SetPath(BPath(&ref).Path());
				}
				// check file menu		
				BMenuItem *menuitem = menu->FindItem(path.Leaf());
				if(menuitem)
				{
					menuitem->SetMarked(true);
					fOldItem = menu->IndexOf(menuitem);	
				}
			}
			break;
		}
	case B_CANCEL:
		{
			Window()->SetFeel(B_MODAL_APP_WINDOW_FEEL);
			
			BMenuField *menufield = cast_as(FindView("filemenu"),BMenuField);
			BMenu *menu = menufield->Menu();
			BMenuItem *item = menu->FindMarked();
			if(::strcmp(item->Label(),"Other…") == 0 )
			{
				item = menu->ItemAt(fOldItem);
				if(item)
				{
					item->SetMarked(true);
					fOldItem = menu->IndexOf(item);	
				}else{
					BString label = "<";
					label << _("none") << ">";
					item = menu->FindItem(label.String());
					item->SetMarked(true);
					fOldItem = menu->IndexOf(item);
				}	
			}
			
			break;
		}
	default:
		BView::MessageReceived(message);
	}
}

/***********************************************************
 * Download Sound
 ***********************************************************/
const char*
HSoundSettingView::DownloadSound()
{
	BMenuField *menufield = cast_as(FindView("filemenu"),BMenuField);
	BMenu *menu = menufield->Menu();
	HMenuItem *item = cast_as(menu->FindMarked(),HMenuItem);
	if(item)
		return item->Path();
	return NULL;
}
/***********************************************************
 * Init menu
 ***********************************************************/
void
HSoundSettingView::SetupMenuField()
{
	BMenuField *menufield = cast_as(FindView("filemenu"),BMenuField);
	BMenu *menu = menufield->Menu();
	HMenuItem *item;
	BMessage *msg;
	
		
	BPath path("/boot/beos/etc/sounds");
	status_t err = B_OK;
	BDirectory dir( path.Path() );
	BEntry entry;
	BPath item_path;
   	while( err == B_OK ){
		err = dir.GetNextEntry( (BEntry*)&entry, TRUE );			
		if( entry.InitCheck() != B_NO_ERROR ){
			break;
		}
		entry.GetPath(&item_path);
		
		if( menu->FindItem(item_path.Leaf()) )
			continue;
			
		msg = new BMessage(M_ITEM_MESSAGE);
		entry_ref ref;
		::get_ref_for_path(item_path.Path(),&ref);
		msg->AddRef("refs",&ref);
		menu->AddItem(item = new HMenuItem(item_path.Leaf(),msg),0);	
		item->SetPath(item_path.Path());	
	}
	
	path.SetTo("/boot/home/config/sounds");
	dir.SetTo(path.Path());
	err = B_OK;
	while( err == B_OK ){
		err = dir.GetNextEntry( (BEntry*)&entry, TRUE );			
		if( entry.InitCheck() != B_NO_ERROR ){
			break;
		}
		entry.GetPath(&item_path);
		
		if( menu->FindItem(item_path.Leaf()) )
			continue;
			
		msg = new BMessage(M_ITEM_MESSAGE);
		entry_ref ref;
		
		::get_ref_for_path(item_path.Path(),&ref);
		msg->AddRef("refs",&ref);
		menu->AddItem(item = new HMenuItem(item_path.Leaf(),msg),0);
		item->SetPath(item_path.Path());		
	}
	
	
	const char *Path;
	((HApp*)be_app)->Prefs()->GetData("snd_download",&Path);

	if( ::strlen(Path) >0 )
	{
		entry_ref ref;
		::get_ref_for_path(Path,&ref);
		
		item = cast_as(menu->FindItem(BPath(Path).Leaf()),HMenuItem); 
		if( !item )
		{
			msg = new BMessage(M_ITEM_MESSAGE);
			msg->AddRef("refs",&ref);
			menu->AddItem(item = new HMenuItem(BPath(Path).Leaf(),msg),0);		
			item->SetMarked(true);
			item->SetPath(Path);
		}else{
			item->SetMarked(true);
		}
	}else{
		BString label = "<";
		label << _("none") << ">";
		BMenuItem *bItem = cast_as(menu->FindItem(label.String()),BMenuItem);
		if(bItem)
			bItem->SetMarked(true);
	}

	fOldItem = menu->IndexOf(menu->FindMarked());	
}

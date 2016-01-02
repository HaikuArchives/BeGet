#include <private/interface/ColumnListView.h>
#include "HListView.h"
#include "HListItem.h"
#include "ResourceUtils.h"
#include "HWindow.h"
#include "IconMenuItem.h"
#include "HPrefs.h"
#include "HApp.h"
#include "MenuUtils.h"
#include "PasswordWindow.h"

#include <PopUpMenu.h>
#include <MenuItem.h>
#include <ClassInfo.h>
#include <private/interface/ColumnTypes.h>


class HStringColumn : public BStringColumn {
	public:
	HStringColumn(const char* str, float size)	: BStringColumn(str,size,20,800,0,B_ALIGN_LEFT) {}

	void
	MouseDown(BColumnListView* parent, BRow* row,
		BField* field, BRect fieldRect,
		BPoint pos, uint32 buttons)
	{
		BPoint point = pos;
		ResourceUtils utils;
		MenuUtils menu_utils;
	// Handling of right click
	if (buttons == B_SECONDARY_MOUSE_BUTTON) {
		HListItem* item = cast_as(row,HListItem);
		BPopUpMenu* theMenu = new BPopUpMenu("RIGHT_CLICK", false, false);
		BFont font(be_plain_font);
		font.SetSize(10);
		theMenu->SetFont(&font);

		menu_utils.AddMenuItem(theMenu
							   , _("Add New Download")
							   , M_ADD_URL
							   , NULL, NULL
							   , 'A', 0, utils.GetBitmapResource('BBMP', "BMP:ADDURL"));

		theMenu->AddSeparatorItem();

		menu_utils.AddMenuItem(theMenu
							   , _("Suspend")
							   , M_SUSPEND, NULL, NULL
							   , 0, 0, utils.GetBitmapResource('BBMP', "BMP:SUSPEND"));
		if (item) {
			theMenu->FindItem(M_SUSPEND)->SetEnabled(item->IsSuspendable());
		} else {
			theMenu->FindItem(M_SUSPEND)->SetEnabled(false);
		}


		menu_utils.AddMenuItem(theMenu
							   , _("Resume")
							   , M_RESUME, NULL, NULL
							   , 0, 0, utils.GetBitmapResource('BBMP', "BMP:RESUME"));
		if (item) {
			theMenu->FindItem(M_RESUME)->SetEnabled(item->IsResumable());
		} else {
			theMenu->FindItem(M_RESUME)->SetEnabled(false);
		}

		theMenu->AddSeparatorItem();

		menu_utils.AddMenuItem(theMenu
							   , _("Stop")
							   , M_STOP, NULL, NULL
							   , 0, 0, utils.GetBitmapResource('BBMP', "BMP:STOP"));
		if (item)
			theMenu->FindItem(M_STOP)->SetEnabled(item->IsStarted());
		else
			theMenu->FindItem(M_STOP)->SetEnabled(false);

		menu_utils.AddMenuItem(theMenu
							   , _("Start")
							   , M_START, NULL, NULL
							   , 0, 0, utils.GetBitmapResource('BBMP', "BMP:CONNECTING"));
		if (item)
			theMenu->FindItem(M_START)->SetEnabled(!item->IsStarted());
		else
			theMenu->FindItem(M_START)->SetEnabled(false);

		theMenu->AddSeparatorItem();

		menu_utils.AddMenuItem(theMenu
							   , _("Delete")
							   , M_DELETE, NULL, NULL
							   , 'T', 0, utils.GetBitmapResource('BBMP', "BMP:TRASH"));
		theMenu->FindItem(M_DELETE)->SetEnabled((item) ? true : false);

		BRect r;
		parent->ConvertToScreen(&pos);
		r.top = pos.y - 5;
		r.bottom = pos.y + 5;
		r.left = pos.x - 5;
		r.right = pos.x + 5;

		BMenuItem* theItem = theMenu->Go(pos, false, true, r);
		if (theItem) {
			BMessage*	aMessage = theItem->Message();
			if (aMessage)
				parent->Window()->PostMessage(aMessage);
		}
		delete theMenu;
	}
	}
};


/***********************************************************
 * Constructor
 ***********************************************************/
HListView::HListView(const char* title)
	: BColumnListView(title,
					 B_WILL_DRAW | B_FRAME_EVENTS | B_NAVIGABLE | B_PULSE_NEEDED,
					 B_NO_BORDER,true) {
	int16 cols[6];
	HPrefs* prefs = ((HApp*)be_app)->Prefs();
	for (int i = 1; i <= 6; i++) {
		BString name = "col";
		name << (int32)i;
		prefs->GetData(name.String(), &cols[i-1]);
	}
	AddColumn(new BBitmapColumn("", 20, 20, 800, B_ALIGN_LEFT),0);
	AddColumn(new HStringColumn(_("Name"), cols[0]),1);
	AddColumn(new HStringColumn(_("Total"), cols[1]),2);
	AddColumn(new HStringColumn(_("Transfered"), cols[2]),3);
	AddColumn(new HStringColumn(_("Average"), cols[3]),4);
	AddColumn(new HStringColumn(_("Estimated"), cols[4]),5);
	AddColumn(new HStringColumn(_("Elapsed"), cols[5]),6);

	//SetSortKey(0);
	SetFont(be_fixed_font);

	//SetSortFunction(CLVEasyItem::CompareItems);
	SetSortingEnabled(true);

	SetInvocationMessage(new BMessage(M_LIST_DBL_CLICKED));
	SetSelectionMessage(new BMessage(M_SELECTION_CHANGED));
}

/***********************************************************
 * Destructor
 ***********************************************************/
HListView::~HListView() {
	HPrefs* prefs = ((HApp*)be_app)->Prefs();
	for (int16 i = 1; i <= 6; i++) {
		BStringColumn* col = (BStringColumn*) ColumnAt(i);

		BString name = "col";
		name << (int32)i;
		int16 width = static_cast<int16>(col->Width());
		prefs->SetData(name.String(), width);
	}
	SetInvocationMessage(NULL);
	SetSelectionMessage(NULL);
}

/***********************************************************
 * MessageReceived
 ***********************************************************/
void
HListView::MessageReceived(BMessage* message) {
	switch (message->what) {
		case M_PASS_MSG: {
				HListItem* item;
				if (message->FindPointer("pointer", (void**)&item) == B_OK) {
					item->SetLoginAndPassword(message->FindString("login"),
											  message->FindString("password"));
					item->Stop();
					item->Start();
				}
				break;
			}
		case B_NODE_MONITOR: {
				int32 opcode;
				if (message->FindInt32("opcode", &opcode) == B_OK) {
					//message->PrintToStream();
					switch (opcode) {
						case B_ENTRY_REMOVED: {
								node_ref nref;
								message->FindInt64("node", &nref.node);
								message->FindInt32("device", &nref.device);
								GoodbyeFile(nref);
								break;
							}
						case B_ENTRY_MOVED: {
								node_ref nref;
								node_ref from_directory;
								node_ref to_directory;
								message->FindInt64("from directory", &from_directory.node);
								message->FindInt64("to directory", &to_directory.node);
								if (from_directory.node == to_directory.node)
									break;

								message->FindInt64("node", &nref.node);
								message->FindInt32("device", &nref.device);

								const char* folder;
								((HApp*)be_app)->Prefs()->GetData("option", &folder);
								BString path = "";
								char* p = strstr(folder, "-P ");
								p += 3;
								while (*p) {
									if (*p == '\0' || *p == ' ')
										break;
									path << *p++;
								}

								node_ref folder_nref;
								BDirectory(path.String()).GetNodeRef(&folder_nref);

								if (from_directory.node == folder_nref.node)
									GoodbyeFile(nref);

								break;
							}
					}
				}
				break;
			}
		default:
			BColumnListView::MessageReceived(message);
	}
}

/***********************************************************
 * GoodbyeFile
 ***********************************************************/
void
HListView::GoodbyeFile(node_ref nref) {
	int32 count = CountRows();

	for (int32 i = 0; i < count; i++) {
		HListItem* item = cast_as(RowAt(i), HListItem);
		if (!item)
			continue;

		if (nref == item->NodeRef()) {
			item->SetForceDelete(true);
			DeleteItem(i);
			break;
		}
	}
}

/***********************************************************
 * AddURL
 ***********************************************************/
void
HListView::AddURL(const char* url, const char* path, uint32 size) {
	HListItem* item = new HListItem(url, path, size);
	AddRow(item);
	fPointerList.AddItem(item);

	bool auto_start;
	bool queue;
	((HApp*)be_app)->Prefs()->GetData("queue", &queue);
	((HApp*)be_app)->Prefs()->GetData("auto_start", &auto_start);
	if (!queue) {
		if (auto_start && item->State() != T_FINISHED)
			item->Start();
	} else {
		int32 max;
		((HApp*)be_app)->Prefs()->GetData("max_transfer", &max);
		if (max > CountDownloadingItems() && auto_start && item->State() != T_FINISHED)
			item->Start();
	}
	AddToSelection(item);
}

/***********************************************************
 * DeletePointers
 ***********************************************************/
void
HListView::DeletePointers() {
	register int32 count = fPointerList.CountItems();

	while (count > 0) {
		delete(HListItem*)fPointerList.RemoveItem(--count);
	}
	Clear();
}

/***********************************************************
 * DeleteItem
 ***********************************************************/
void
HListView::DeleteItem(int32 index) {
	HListItem* item = cast_as(RowAt(index), HListItem);
	fPointerList.RemoveItem(item);
	RemoveRow(item);
	delete item;

	HApp* app = cast_as(be_app, HApp);
	bool queue;

	int32 max_transfer;
	app->Prefs()->GetData("queue", &queue);
	app->Prefs()->GetData("max_transfer", &max_transfer);

	if (queue) {
		int32 num_task = max_transfer - CountDownloadingItems();
		if (num_task > 0) {
			int32 count = CountRows();
			for (int32 i = 0; i < count; i++) {
				item = cast_as(RowAt(i),HListItem);
				if (!item->IsStarted() && item->State() != T_FINISHED && item->State() != T_NOTFOUND) {
					item->Start();
					num_task--;
				}
				if (num_task <= 0)
					break;
			}
		}
	}
}

/***********************************************************
 * CountDownloadingItems
 ***********************************************************/
int32
HListView::CountDownloadingItems() {
	int32 result = 0;
	int32 count = CountRows();
	for (int32 i = 0; i < count; i++) {
		HListItem* item = cast_as(RowAt(i),HListItem);
		if (!item)
			continue;
		if (item->IsStarted())
			result++;
	}
	return result;
}

/***********************************************************
 * MouseDown
 ***********************************************************/
void
HListView::MouseDown(BPoint pos) {
	int32 buttons = 0;
	ResourceUtils utils;
	BPoint point = pos;
	MenuUtils menu_utils;
	Window()->CurrentMessage()->FindInt32("buttons", &buttons);
	this->MakeFocus(true);

	// Handling of right click
	if (buttons == B_SECONDARY_MOUSE_BUTTON) {
		int32 sel = -1;
		if(RowAt(pos) != NULL)
			sel = 0;
		HListItem* item = cast_as(RowAt(pos),HListItem);
		BPopUpMenu* theMenu = new BPopUpMenu("RIGHT_CLICK", false, false);
		BFont font(be_plain_font);
		font.SetSize(10);
		theMenu->SetFont(&font);

		menu_utils.AddMenuItem(theMenu
							   , _("Add New Download")
							   , M_ADD_URL
							   , NULL, NULL
							   , 'A', 0, utils.GetBitmapResource('BBMP', "BMP:ADDURL"));

		theMenu->AddSeparatorItem();

		menu_utils.AddMenuItem(theMenu
							   , _("Suspend")
							   , M_SUSPEND, NULL, NULL
							   , 0, 0, utils.GetBitmapResource('BBMP', "BMP:SUSPEND"));
		if (item) {
			theMenu->FindItem(M_SUSPEND)->SetEnabled(item->IsSuspendable());
		} else {
			theMenu->FindItem(M_SUSPEND)->SetEnabled(false);
		}


		menu_utils.AddMenuItem(theMenu
							   , _("Resume")
							   , M_RESUME, NULL, NULL
							   , 0, 0, utils.GetBitmapResource('BBMP', "BMP:RESUME"));
		if (item) {
			theMenu->FindItem(M_RESUME)->SetEnabled(item->IsResumable());
		} else {
			theMenu->FindItem(M_RESUME)->SetEnabled(false);
		}

		theMenu->AddSeparatorItem();

		menu_utils.AddMenuItem(theMenu
							   , _("Stop")
							   , M_STOP, NULL, NULL
							   , 0, 0, utils.GetBitmapResource('BBMP', "BMP:STOP"));
		if (item)
			theMenu->FindItem(M_STOP)->SetEnabled(item->IsStarted());
		else
			theMenu->FindItem(M_STOP)->SetEnabled(false);

		menu_utils.AddMenuItem(theMenu
							   , _("Start")
							   , M_START, NULL, NULL
							   , 0, 0, utils.GetBitmapResource('BBMP', "BMP:CONNECTING"));
		if (sel >= 0)
			theMenu->FindItem(M_START)->SetEnabled(!item->IsStarted());
		else
			theMenu->FindItem(M_START)->SetEnabled(false);

		theMenu->AddSeparatorItem();

		menu_utils.AddMenuItem(theMenu
							   , _("Delete")
							   , M_DELETE, NULL, NULL
							   , 'T', 0, utils.GetBitmapResource('BBMP', "BMP:TRASH"));
		theMenu->FindItem(M_DELETE)->SetEnabled((sel >= 0) ? true : false);

		BRect r;
		ConvertToScreen(&pos);
		r.top = pos.y - 5;
		r.bottom = pos.y + 5;
		r.left = pos.x - 5;
		r.right = pos.x + 5;

		BMenuItem* theItem = theMenu->Go(pos, false, true, r);
		if (theItem) {
			BMessage*	aMessage = theItem->Message();
			if (aMessage)
				this->Window()->PostMessage(aMessage);
		}
		delete theMenu;
	} else
		BColumnListView::MouseDown(point);
}

/***********************************************************
 * FindNextSelection
 ***********************************************************/
int32
HListView::FindNextSelection(int32 index) {
	int32 count = CountRows();

	for (int32 i = index + 1; i < count; i++) {
		HListItem* item = cast_as(RowAt(i), HListItem);
		if (item->IsSelected())
			return i;
	}
	return -1;
}

/***********************************************************
 * KeyDown
 ***********************************************************/
void
HListView::KeyDown(const char* bytes, int32 numBytes) {
	if (numBytes == 1 && bytes[0] == B_DELETE) {
		/*int32 sel = CurrentSelection();

		while(sel >= 0)
		{
			HListItem *item = cast_as(ItemAt(sel),HListItem);
			item->SetForceDelete(true);
			DeleteItem(sel);
			sel = FindNextSelection(sel-1);
		}*/
		Window()->PostMessage('MDEL');
	}
	BColumnListView::KeyDown(bytes, numBytes);
}

/***********************************************************
 * Pulse
 ***********************************************************/
void
HListView::Pulse() {
	register int32 count = CountRows();

	while (count > 0) {
		HListItem* item = cast_as(RowAt(--count), HListItem);
		item->RefreshTime();
		if (item->IsDirty()) {
			InvalidateRow(item);
			item->SetDirty(false);
		}
	}
}

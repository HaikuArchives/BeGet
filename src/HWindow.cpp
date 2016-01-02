#include "HWindow.h"
#include "MenuUtils.h"
#include "SplitPane.h"
#include "ResourceUtils.h"
#include "HToolbar.h"
#include "HListView.h"
#include "HLogView.h"
#include "HListItem.h"
#include "HApp.h"
#include "HAddUrlDlg.h"
#include "RectUtils.h"
#include "HCaption.h"
#include "HToolbarButton.h"
#include "HSettingWindow.h"
#include "URLSetting.h"
#include "PasswordWindow.h"
#include "TrackerUtils.h"

#include <ClassInfo.h>
#include <NodeMonitor.h>
#include <MenuBar.h>
#include <Application.h>
#include <iostream>
#include <NodeInfo.h>
#include <Deskbar.h>
#include <Debug.h>
#include <GroupLayout.h>
#include <PictureButton.h>
#include <private/shared/ToolBar.h>

filter_result f_click(BMessage* ioMessage, BHandler** ioTarget, BMessageFilter* inFilter);

/***********************************************************
 * Constructor
 ***********************************************************/
HWindow::HWindow(BRect rect , const char* name)
	: BWindow(rect, name, B_DOCUMENT_WINDOW, 0) {
	InitGUI();
	bool deskbar;
	((HApp*)be_app)->Prefs()->GetData("deskbar", &deskbar);
	if (deskbar)
		InstallToDeskbar();
	// set window size limit
	float min_width, min_height, max_width, max_height;
	GetSizeLimits(&min_width, &max_width, &min_height, &max_height);
	min_width = 300;
	min_height = 150;
	SetSizeLimits(min_width, max_width, min_height, max_height);
	// set pulse rate
	SetPulseRate(1000000 * 0.5);
	// load old items
	fURLSetting = new URLSetting();
	int32 count = fURLSetting->CountItems();
	for (int32 i = 0; i < count; i++)
		fListView->AddURL(fURLSetting->FindURL(i)
						  , fURLSetting->FindPath(i)
						  , fURLSetting->FindSize(i));
	// start watching clipboard
	fListView->MakeFocus(true);
	be_clipboard->StartWatching(this);
	AddCommonFilter(new BMessageFilter(B_MOUSE_DOWN,f_click));
}

/***********************************************************
 * Destructor
 ***********************************************************/
HWindow::~HWindow() {
	be_clipboard->StopWatching(this);
	delete fURLSetting;
	stop_watching(this);
}

/***********************************************************
 * InitGUI
 ***********************************************************/
void
HWindow::InitGUI() {
	BGroupLayout* bgLayout=new BGroupLayout(B_VERTICAL);
	BView* bg = new BView(Bounds(), "bg", B_FOLLOW_ALL, 0);
	bg->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	bg->SetLayout(bgLayout);

	ResourceUtils utils;
	toolbar = new BPrivate::BToolBar();
	toolbar->AddAction(new BMessage(M_ADD_URL),this,utils.GetBitmapResource('BBMP', "BMP:ADDURL"),_("Add New Download"));
	toolbar->AddSeparator();
	toolbar->AddAction(new BMessage(M_DELETE),this,utils.GetBitmapResource('BBMP', "BMP:TRASH"), _("Delete Selected Item"));

	toolbar->AddSeparator();
	toolbar->AddAction(new BMessage(M_STOP),this,utils.GetBitmapResource('BBMP', "BMP:STOP"), _("Stop"));
	toolbar->AddAction(new BMessage(M_START),this,utils.GetBitmapResource('BBMP', "BMP:CONNECTING"), _("Start"));

	fListView = new HListView("downlist");

	fLogView = new HLogView("logview", B_WILL_DRAW);
	BScrollView* scroll = new BScrollView("scroll", fLogView,
										  B_WILL_DRAW | B_FRAME_EVENTS, false, true, B_FANCY_BORDER);

	bgLayout->AddView(InitMenu());
	bgLayout->AddView(toolbar,2);
	bgLayout->AddView(fListView,50);
	bgLayout->AddView(scroll,15);


	/****************** StatusBar ***********************/
	BBox* box = new BBox("status", B_WILL_DRAW);
	bgLayout->AddView(box,5);

	HCaption* view = new HCaption(box->Bounds(), "info", fListView);
	box->AddChild(view);

	AddChild(bg);
}

/***********************************************************
 *
 ***********************************************************/
BMenuBar*
HWindow::InitMenu() {
	BMenuBar* menuBar = new BMenuBar("MENUBAR");
	BMenu* menu;
	MenuUtils utils;
	ResourceUtils res_utils;

	// file menu
	menu = new BMenu(_("File"));
	utils.AddMenuItem(menu, _("Add New Download"), M_ADD_URL, NULL, this, 'A', 0, res_utils.GetBitmapResource('BBMP', "BMP:ADDURL"));
	utils.AddMenuItem(menu, _("Open Downloads Folder"), M_OPEN_FOLDER, NULL, this, 'F', 0, res_utils.GetBitmapResource('BBMP', "BMP:FOLDER"));
	menu->AddSeparatorItem();
	BString label = _("Preferences");
	label += "…";
	utils.AddMenuItem(menu, label.String(), M_PREFS, this, this, 'P', 0);
	menu->AddSeparatorItem();
	utils.AddMenuItem(menu, _("About BeGet…"), B_ABOUT_REQUESTED, NULL, be_app, 0, 0);
	menu->AddSeparatorItem();
	utils.AddMenuItem(menu, _("Quit"), B_QUIT_REQUESTED, NULL, this, 'Q', 0);
	menuBar->AddItem(menu);
	// Operation menu
	menu = new BMenu(_("Operations"));
	/*
	utils.AddMenuItem(menu,_("Suspend"),M_SUSPEND,NULL,this,0,0,res_utils.GetBitmapResource('BBMP',"BMP:SUSPEND"));
	utils.AddMenuItem(menu,_("Resume"),M_RESUME,NULL,this,0,0,res_utils.GetBitmapResource('BBMP',"BMP:RESUME"));
	menu->AddSeparatorItem();
	*/
	utils.AddMenuItem(menu, _("Stop"), M_STOP, NULL, this, 0, 0, res_utils.GetBitmapResource('BBMP', "BMP:STOP"));
	utils.AddMenuItem(menu, _("Start"), M_START, NULL, this, 0, 0, res_utils.GetBitmapResource('BBMP', "BMP:CONNECTING"));
	menu->AddSeparatorItem();
	utils.AddMenuItem(menu, _("Delete Selected Item"), M_DELETE, NULL, this, 'T', 0, res_utils.GetBitmapResource('BBMP', "BMP:TRASH"));
	utils.AddMenuItem(menu, _("Delete Finished Items"), M_DELETE_FINISHED, NULL, this, 'T', B_SHIFT_KEY, res_utils.GetBitmapResource('BBMP', "BMP:TRASHFINISHED"));
	menuBar->AddItem(menu);
	return menuBar;
}

/***********************************************************
 * InstallToDeskbar
 ***********************************************************/
void
HWindow::InstallToDeskbar() {
	BDeskbar deskbar;

	if (deskbar.HasItem("BeGet") == false) {
		BRoster roster;
		entry_ref ref;
		roster.FindApp("application/x-vnd.takamatsu.beget", &ref);
		int32 id;
		deskbar.AddItem(&ref, &id);
	}
}

/***********************************************************
 * Watch
 ***********************************************************/
void
HWindow::Watch() {
	bool watch;
	bool confirm;
	((HApp*)be_app)->Prefs()->GetData("watch", &watch);
	if (!watch)
		return;
	((HApp*)be_app)->Prefs()->GetData("watch_confirm", &confirm);

	BMessage* clip = NULL;
	if (be_clipboard->Lock()) {
		clip = be_clipboard->Data();
		if (clip) {
			int32 len;
			const char* text;
			if (clip->FindData("text/plain", B_MIME_TYPE, 0, (const void**)&text, &len) == B_OK) {
				BString url;
				url.SetTo(text, len);
				const char* exts;
				((HApp*)be_app)->Prefs()->GetData("watch_ext", &exts);
				int32 len = strlen(exts);
				BString ext = "";
				for (register int32 i = 0; i < len; i++) {
					if (exts[i] != ' ' && i != len - 1)
						ext << exts[i];
					else {
						if (i == len - 1)
							ext << exts[i];
						if (CheckExt(url.String(), ext.String())) {
							if (confirm)
								OpenAddUrlDlg(url.String());
							else
								fListView->AddURL(url.String());
						}
						ext = "";
					}
				}
			}
		}
		be_clipboard->Unlock();
	}
}


/***********************************************************
 * CheckExt
 ***********************************************************/
bool
HWindow::CheckExt(const char* in_url , const char* in_ext) {
	BString url(in_url);
	BString ext = ".";
	ext << in_ext;
	int32 extlen = ext.Length();
	int32 find = url.IFindLast(ext);
	if (find == url.Length() - extlen)
		return true;

	return false;
}
/***********************************************************
 * MessageReceived
 ***********************************************************/
void
HWindow::MessageReceived(BMessage* message) {
	HListItem* item = cast_as(fListView->CurrentSelection(), HListItem);
	int32 sel = fListView->IndexOf(item);

	if(item) {
		toolbar->SetActionEnabled(M_DELETE,true);
		toolbar->SetActionEnabled(M_STOP,item->IsStarted());
		toolbar->SetActionEnabled(M_START,!item->IsStarted());
	}else{
		toolbar->SetActionEnabled(M_DELETE,false);
		toolbar->SetActionEnabled(M_STOP,false);
		toolbar->SetActionEnabled(M_START,false);
	}

	switch (message->what) {
		case B_REFS_RECEIVED: {
				const char* url;
				if (message->FindString("be:url", &url) == B_OK)
					fListView->AddURL(url);
				break;
			}
			// clipboard
		case B_CLIPBOARD_CHANGED: {
				Watch();
				break;
			}
			// list dbl clicked
		case M_LIST_DBL_CLICKED: {
				HListItem* sel = cast_as(fListView->CurrentSelection(), HListItem);
				if (sel) {
					const char* path = sel->FilePath();
					if (::strlen(path) != 0 && sel->State() == T_FINISHED) {
						entry_ref ref;
						::get_ref_for_path(path, &ref);
						be_roster->Launch(&ref);
					}
				}
				break;
			}
			// open downloads folder
		case M_OPEN_FOLDER: {
				const char* path;
				((HApp*)be_app)->Prefs()->GetData("option", &path);
				BString p = path;
				int32 index = p.FindFirst("-P ");
				if (index != B_ERROR) {
					index += 3;
					int32 len = p.FindFirst(" ", index);
					if (len == B_ERROR)
						len = p.Length();
					BString str;
					p.CopyInto(str, index, len);

					entry_ref ref;
					::get_ref_for_path(str.String(), &ref);
					TrackerUtils().OpenFolder(ref);
				}
				break;
			}
			// show dowload dialog
		case M_ADD_URL: {
				OpenAddUrlDlg(NULL);
				break;
			}
			// Add download
		case M_REAL_ADD: {
				const char* text;
				int32 count;
				type_code type;
				message->GetInfo("url", &type, &count);
				for (int32 i = 0; i < count; i++) {
					if (message->FindString("url", i, &text) == B_OK)
						fListView->AddURL(text);
				}
				break;
			}
			// Settings
		case M_PREFS: {
				RectUtils utils;
				HSettingWindow* win = new HSettingWindow(utils.CenterRect(400, 300));
				win->Show();
				break;
			}
			// Delete
		case M_DELETE: {
				int32 old_selection = sel;
				while (sel >= 0) {
					item = cast_as(fListView->RowAt(sel), HListItem);
					item->SetForceDelete(true);
					fListView->DeleteItem(sel);
					sel = fListView->FindNextSelection(sel - 1);
				}
				if (old_selection >= 0) {
					HListItem* oldItem = cast_as(fListView->RowAt(old_selection), HListItem);
					fListView->AddToSelection(oldItem);
				}
				break;
			}
			// Delete finished items
		case M_DELETE_FINISHED: {
				int32 count = fListView->CountRows();
				for (int32 i = 0; i < count; i ++) {
					item = cast_as(fListView->RowAt(i), HListItem);
					if (!item)
						continue;
					if (item->State() == T_FINISHED)
						fListView->DeleteItem(i);
				}
				break;
			}
			// Suspend
		case M_SUSPEND: {
				while (item) {
					item->Suspend();
					sel = fListView->FindNextSelection(sel);
					if (sel < 0)
						item = NULL;
					else
						item = cast_as(fListView->RowAt(sel), HListItem);
				}
				break;
			}
			// Resume
		case M_RESUME: {
				while (item) {
					item->Resume();
					sel = fListView->FindNextSelection(sel);
					if (sel < 0)
						item = NULL;
					else
						item = cast_as(fListView->RowAt(sel), HListItem);
				}
				break;
			}
			// Start
		case M_START: {
				bool queue;
				int32 max_transfer;
				((HApp*)be_app)->Prefs()->GetData("queue", &queue);
				((HApp*)be_app)->Prefs()->GetData("max_transfer", &max_transfer);

				while (item) {
					if (!queue)
						item->Start();
					else {
						if (max_transfer > fListView->CountDownloadingItems())
							item->Start();
					}
					sel = fListView->FindNextSelection(sel);
					if (sel < 0)
						item = NULL;
					else
						item = cast_as(fListView->RowAt(sel), HListItem);
				}
				break;
			}
			// Stop
		case M_STOP: {
				int32 count=fListView->CountRows();
				for(int i=0;i<count;i++) {
					HListItem* item = cast_as(fListView->RowAt(i), HListItem);
					item->Stop();
				}
				break;
			}
			// auto delete
		case M_DELETE_ITEM: {
				void* data;
				if (message->FindPointer("pointer", &data) == B_OK) {
					HListItem* item = static_cast<HListItem*>(data);
					if (item) {
						int32 index = fListView->IndexOf(item);
						fListView->DeleteItem(index);
					}
				}
				break;
			}
			// add watch node
		case M_ADD_WATCH_NODE: {
				entry_ref ref;
				if (message->FindRef("refs", &ref) == B_OK) {
					BEntry entry(&ref);
					node_ref nref;
					entry.GetNodeRef(&nref);
					if (::watch_node(&nref, B_WATCH_NAME, fListView, this) == B_OK) {
						PRINT(("Added to node watcher"));
						PRINT((BPath(&ref).Path()));
					}
				}
				break;
			}
			// Sel Changed
		case M_SELECTION_CHANGED: {
				if (!item)
					fLogView->SetText("");
				else
					fLogView->SetText(item->Log());
				fLogView->ScrollToEnd();
				break;
			}
		case M_PASS_MSG:
			PostMessage(message, fListView);
			break;
			// Update toolbar buttons
		default: {
			if (message->WasDropped())
				WhenDropped(message);
			else
				BWindow::MessageReceived(message);

		}
	}
}

/***********************************************************
 * Pulse
 ***********************************************************/
void
HWindow::Pulse() {
	HListItem* sel = cast_as(fListView->CurrentSelection(), HListItem);
	if (sel) {
		BString log = sel->Log();

		int32 old_len = fLogView->TextLength();
		if (log.Length() < old_len) {
			fLogView->SetText(log.String());
			fLogView->ScrollToEnd();
			return;
		}
		if (log.Length() != old_len) {
			fLogView->Select(fLogView->TextLength(), fLogView->TextLength());
			fLogView->Insert(&log[ old_len ]);
			fLogView->ScrollToEnd();
		}
	}
}

/***********************************************************
 * MenusBegginging.
 ***********************************************************/
void
HWindow::MenusBeginning() {
	HListItem* sel = cast_as(fListView->CurrentSelection(),HListItem);
	bool selection = true;
	if (!sel)
		selection = false;

	KeyMenuBar()->FindItem(M_DELETE)->SetEnabled(selection);

	if (selection) {
		//KeyMenuBar()->FindItem(M_SUSPEND)->SetEnabled(item->IsSuspendable() );
		//KeyMenuBar()->FindItem(M_RESUME)->SetEnabled( item->IsResumable() );
		KeyMenuBar()->FindItem(M_STOP)->SetEnabled(sel->IsStarted());
		KeyMenuBar()->FindItem(M_START)->SetEnabled(!sel->IsStarted());
	} else {
		//KeyMenuBar()->FindItem(M_SUSPEND)->SetEnabled(selection);
		//KeyMenuBar()->FindItem(M_RESUME)->SetEnabled(selection);
		KeyMenuBar()->FindItem(M_STOP)->SetEnabled(selection);
		KeyMenuBar()->FindItem(M_START)->SetEnabled(selection);
	}

}

/***********************************************************
 * DispatchMessage
 ***********************************************************/
void
HWindow::DispatchMessage(BMessage* message, BHandler* target) {
	if (message->what == B_PULSE)
		Pulse();
	BWindow::DispatchMessage(message, target);
}

/***********************************************************
 * WhenDropped
 ***********************************************************/
void
HWindow::WhenDropped(BMessage* message) {
	//message->PrintToStream();
	const char* text = NULL;
	ssize_t text_len;
	entry_ref ref;
	if (message->FindData("text/plain", B_MIME_TYPE, (const void**)&text, &text_len) == B_OK) {
		BString str;
		str.Append(text, text_len);
		OpenAddUrlDlg(str.String());
	} else if (message->FindString("be:url", &text) == B_OK) {
		fListView->AddURL(text);
	} else if (message->FindRef("refs", &ref) == B_OK) { // for bookmarks
		BFile file(&ref, B_READ_ONLY);
		BNodeInfo ninfo(&file);
		if (ninfo.InitCheck() != B_OK)
			return;
		char type[B_MIME_TYPE_LENGTH+1];
		ninfo.GetType(type);
		if (::strcmp(type, "application/x-vnd.Be-bookmark") == 0) {
			BString url;
			file.ReadAttrString("META:url", &url);
			fListView->AddURL(url.String());
		} else if (::strcmp(type, "text/plain") == 0) {
			off_t size;
			file.GetSize(&size);
			char* tmp = new char[size+1];
			size = file.Read(tmp, size);
			OpenAddUrlDlg(tmp);

			delete[] tmp;
		} else {
			attr_info ainfo;
			if (file.GetAttrInfo("META:url", &ainfo) == B_OK) {
				BString url;
				file.ReadAttrString("META:url", &url);
				fListView->AddURL(url.String());
			}
		}
	}
}

/***********************************************************
 * Open Add url dlg
 ***********************************************************/
void
HWindow::OpenAddUrlDlg(const char* url) {
	RectUtils utils;
	HAddUrlDlg* dlg = new HAddUrlDlg(utils.CenterRect(400, 200), url);
	dlg->Show();
}

/***********************************************************
 * QuitRequested
 ***********************************************************/
bool
HWindow::QuitRequested() {
	int32 count = fListView->CountRows();
	fURLSetting->MakeEmpty();
	for (int32 i = 0; i < count; i++) {
		HListItem* item = cast_as(fListView->RowAt(i), HListItem);
		if (item)
			fURLSetting->AddURL(item->URL(), item->FilePath(), item->TotalSize());
	}

	fListView->DeletePointers();

	BDeskbar deskbar;
	if (deskbar.HasItem("BeGet")) {
		deskbar.RemoveItem("BeGet");
	}

	/*BPoint pos = fHSplitter->GetBarPosition();
	int32 y = static_cast<int32>(pos.y);
	((HApp*)be_app)->Prefs()->SetData("hbar_pos", y);*/

	((HApp*)be_app)->Prefs()->SetData("main_window", Frame());
	be_app->PostMessage(B_QUIT_REQUESTED);
	return BWindow::QuitRequested();
}

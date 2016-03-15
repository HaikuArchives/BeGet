#include "HListItem.h"
#include "ResourceUtils.h"
#include "HApp.h"
#include "HWindow.h"
#include "PasswordWindow.h"
#include "RectUtils.h"
#include "TrackerUtils.h"

#include <Alert.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <Path.h>
#include <Beep.h>
#include <Roster.h>
#include <stdio.h>
#include <Debug.h>
#include <sys/socket.h>

/***********************************************************
 * Constructor
 ***********************************************************/
HListItem::HListItem(const char* url, const char* path, uint32 size)
	: BRow()
	, fElapsed(0)
	, fTotalSize(size)
	, fReceivedSize(0)
	, fURL(url)
	, fProcess(-1)
	, fLog("")
	, fWatcherThread(-1)
	, fWGetThread(-1)
	, fContinueWatching(false)
	, fState(T_NOT_CONNECTED)
	, fDirty(false)
	, fOldSize(0)
	, fLocalPath(path)
	, fForceDelete(false)
	, fLogin("")
	, fPassword("") {
	SetState(T_NOT_CONNECTED);
	// set name field to url
	SetName(url);
	SetField(new BStringField(""),2);
	SetField(new BStringField(""),3);
	SetField(new BStringField(""),4);
	SetField(new BStringField(""),5);
	SetField(new BStringField(""),6);
	// check exist file
	if (path)
		CheckSize();
}

/***********************************************************
 * Destructor
 ***********************************************************/
HListItem::~HListItem() {
	// close process pipe
	close(fProcess);
	// move to trash downloaded file
	MoveToTrash();
	// stop watching wget output
	StopWatching();
	// kill wget thread
	::kill_thread(fWGetThread);
}

/***********************************************************
 * StopWatching
 ***********************************************************/
void
HListItem::StopWatching() {
	// wait for finishing thread
	fContinueWatching = false;

	status_t err;
	if (fWatcherThread >= 0)
		::wait_for_thread(fWatcherThread, &err);
	//
	fWatcherThread = -1;
}

/***********************************************************
 * IsSuspendable
 ***********************************************************/
bool
HListItem::IsSuspendable() {
	// return false if wget thread is not running
	if (fWGetThread < 0)
		return false;

	// get running thread info. if it has already suspended, return false
	thread_info info;
	::get_thread_info(fWGetThread, &info);
	bool rc = true;
	if (info.state == B_THREAD_SUSPENDED)
		rc = false;
	return rc;
}

/***********************************************************
 * IsResumable
 ***********************************************************/
bool
HListItem::IsResumable() {
	if (fWGetThread < 0)
		return false;

	return !IsSuspendable();
}

/***********************************************************
 * IsStarted
 ***********************************************************/
bool
HListItem::IsStarted() {
	if (fWGetThread < 0)
		return false;

	return true;
}

/***********************************************************
 * Suspend
 ***********************************************************/
void
HListItem::Suspend() {
	// store old state
	fOldState = State();
	// set state to suspended
	SetState(T_SUSPEND);
	// stop watching wget output
	StopWatching();
	// suspend thread
	::suspend_thread(fWGetThread);
}

/***********************************************************
 * Resume
 ***********************************************************/
void
HListItem::Resume() {
	// set state to old state
	SetState((States)fOldState);
	// resume thread
	::resume_thread(fWGetThread);
	// start watching wget output
	StartWatching();
}

/***********************************************************
 * Stop
 ***********************************************************/
void
HListItem::Stop() {
	::kill_thread(fWGetThread);

	SetState(T_NOT_CONNECTED);

	fWGetThread = -1;

	StopWatching();
}

/***********************************************************
 * Start
 ***********************************************************/
void
HListItem::Start() {
	StartWatching();
	::kill_thread(fWGetThread);
	Wget();
}
/***********************************************************
 * SetTotalSize
 ***********************************************************/
void
HListItem::SetTotalSize(uint32 size) {
	BString str;
	str << size;
	SetField(new BStringField(str),2);
	fTotalSize = size;
	fDirty = true;
}

/***********************************************************
 * RefreshTime
 ***********************************************************/
void
HListItem::RefreshTime() {
	time_t timet = time(NULL);
	time_t new_time = static_cast<time_t>(difftime(timet, fStartTime));

	if (fElapsed != new_time && fState != T_NOTFOUND && fState != T_FINISHED && fState != T_NOT_CONNECTED) {

		BString str("");

		if (fElapsed < 0)
			fElapsed = 0;
		FormatTime(fElapsed, str);

		SetField(new BStringField(str),6);
		fDirty = true;
		fElapsed = new_time;
	}
}

/***********************************************************
 * SetReceivedSize
 ***********************************************************/
void
HListItem::SetReceivedSize(uint32 diff_size) {
	BString str;

	off_t size;
	entry_ref ref;
	::get_ref_for_path(fLocalPath.String(), &ref);
	BEntry entry(&ref);
	if (entry.GetSize(&size) == B_OK) {
		diff_size = size - fReceivedSize;
		fReceivedSize = size;
	} else
		fReceivedSize += diff_size;

	str << fReceivedSize;
	if (fTotalSize != 0)
		str << " (" << fReceivedSize * 100 / fTotalSize << "%)";
	else
		str << " (" << 0 << "%)";
	SetField(new BStringField(str),3);


	fOldSize += diff_size;
	str = "";

	if (fElapsed > 0) {
		char tmp[15];
		double ave = fOldSize / (double)fElapsed;
		/*if(ave < 1024)
			str << (int32)ave << " Bytes";
		else if(ave > 1024 && ave < 1048510)
			str << (int32)ave/1024 << " KB";
		else if(ave > 1048510)
			str << (int32)ave/1048510 << " MB";
		*/
		if (ave < 1024)
			sprintf(tmp, "%6.2f Bytes", ave);
		else if (ave > 1024 && ave < 1048510)
			sprintf(tmp, "%6.2f KB", ave / 1024.0);
		else if (ave > 1048510)
			sprintf(tmp, "%6.2f MB", ave / 1048510.0);

		SetField(new BStringField(tmp),4);

		uint32 remains = fTotalSize - fReceivedSize;
		int32 rtime = static_cast<int32>(remains / ave);
		if (rtime < 0)
			rtime = 0;
		str = "";

		FormatTime(rtime, str);
		SetField(new BStringField(str),5);
	}


	fDirty = true;
}

/***********************************************************
 * SetStatus
 ***********************************************************/
void
HListItem::SetState(States state) {
	fState = state;
	ResourceUtils utils;
	BBitmap* bitmap = NULL;
	switch (state) {
		case T_NOT_CONNECTED:
			bitmap = utils.GetBitmapResource('BBMP', "BMP:STOP");
			break;
		case T_SUSPEND:
			bitmap = utils.GetBitmapResource('BBMP', "BMP:SUSPEND");
			break;
		case T_CONNECTING:
			bitmap = utils.GetBitmapResource('BBMP', "BMP:CONNECTING");
			break;
		case T_CONNECTED:
			break;
		case T_NOTFOUND: {
				bitmap = utils.GetBitmapResource('BBMP', "BMP:ERROR");
				::kill_thread(fWGetThread);

				fWGetThread = -1;
				fContinueWatching = false;
				fWatcherThread = -1;
				break;
			}
		case T_RECEIVING:
			bitmap = utils.GetBitmapResource('BBMP', "BMP:DOWNLOADING");
			break;
		case T_FINISHED: {
				bitmap = utils.GetBitmapResource('BBMP', "BMP:FINISHED");
				// set mime type
				if (fLocalPath.Length() > 0) {
					BString cmd;
					cmd << "mimeset \"" << fLocalPath << "\"";
					::system(cmd.String());
				}
				// play sound
				be_app->PostMessage(M_SOUND_DOWNLOAD);
				// launch
				bool auto_launch;
				((HApp*)be_app)->Prefs()->GetData("auto_launch", &auto_launch);
				if (fLocalPath.Length() > 0 && auto_launch) {
					entry_ref ref;
					::get_ref_for_path(fLocalPath.String(), &ref);
					be_roster->Launch(&ref);
				}
				bool del;
				// Delete when downloading has been finished
				((HApp*)be_app)->Prefs()->GetData("auto_delete", &del);
				if (del) {
					BMessage msg(M_DELETE_ITEM);
					msg.AddPointer("pointer", this);
					((HApp*)be_app)->Window()->PostMessage(&msg);
					PRINT(("SEND DELETE\n"));
				}
				break;
			}
	}

	if (bitmap) {
		SetField(new BBitmapField(bitmap),0);
	}
	delete bitmap;
	fDirty = true;
}
/***********************************************************
 * SetName
 ***********************************************************/
void
HListItem::SetName(const char* name) {
	SetField(new BStringField(name),1);
	fDirty = true;
}

/***********************************************************
 * Log
 ***********************************************************/
const char*
HListItem::Log() const {
	return fLog.String();
}

/***********************************************************
 * URL
 ***********************************************************/
const char*
HListItem::URL() const {
	return fURL.String();
}
/***********************************************************
 * StartProcess
 ***********************************************************/
void
HListItem::StartWatching() {
	fWatcherThread = ::spawn_thread(Watcher_thread, "LOG_WATCH_THREAD", B_LOW_PRIORITY, this);
	::resume_thread(fWatcherThread);
}



/***********************************************************
 * CheckSize
 ***********************************************************/
void
HListItem::CheckSize() {
	BFile file(fLocalPath.String(), B_READ_ONLY);
	if (file.InitCheck() == B_OK) {
		off_t size;
		file.GetSize(&size);
		if (fTotalSize <= size)
			SetState(T_FINISHED);
		SetName(BPath(fLocalPath.String()).Leaf());
		SetTotalSize(fTotalSize);
		SetReceivedSize(size);
		RegistNodeMonitor();
	}
}

/***********************************************************
 * watcher_thread
 ***********************************************************/
int32
HListItem::Watcher_thread(void* data) {
	HListItem* item = (HListItem*)data;
	item->Watcher();
	return 0;
}


/***********************************************************
 * watcher
 ***********************************************************/
void
HListItem::Watcher() {
	int total_read = 0, pos = 0;
	BString line = "";
	char c = 0;
	fContinueWatching = true;

	while (fContinueWatching) {
		if (fState == T_FINISHED)
			break;
		if (fProcess > 0)
			total_read = read(fProcess, &c, 1);
		else
			::snooze(1000000);
		if (total_read > 0) {
			fLog.Append(c, total_read);
			pos = 0;
			// analize wget log
			if (fState == T_RECEIVING && c == '.') {
				ProcessLogLine(&c);
			} else {
				if (c == '\n') {
					ProcessLogLine(line.String());
					line.SetTo("");
				} else
					line += c;
				if (strstr(line.String(), "K ->") != NULL && T_CONNECTED)
					SetState(T_RECEIVING);
			}
		} else
			::snooze(100000);
	}
}

/***********************************************************
 * ProcessLogLine
 ***********************************************************/
void
HListItem::ProcessLogLine(const char* line) {
	switch (fState) {
		case T_FINISHED:
		case T_CONNECTING: {
				if (strstr(line, "connected!") != NULL) {
					SetState(T_CONNECTED);
				}
				if (strstr(line, "Host not found.") != NULL) {
					SetState(T_NOTFOUND);
					//beep();
					//(new BAlert("","Hot font found.","OK"))->Go();
				}
				// for localpath
				char* p = strstr(line, "=> `");
				if (p != NULL) {
					p += 4;
					fLocalPath = p;
					fLocalPath.Truncate(fLocalPath.Length() - 1);
					SetName(BPath(fLocalPath.String()).Leaf());
					RegistNodeMonitor();
				}
				if (strstr(line, "' saved [") != NULL) {
					bool del;

					((HApp*)be_app)->Prefs()->GetData("auto_delete", &del);
					if (del) {
						BMessage msg(M_DELETE_ITEM);
						msg.AddPointer("pointer", this);
						((HApp*)be_app)->Window()->PostMessage(&msg);
						PRINT(("SEND DELETE\n"));
					}
				}
				break;
			}
		case T_CONNECTED: {
				char* p = strstr(line, "Length:");
				if (p != NULL) {
					p += 8;
					uint32 tot_size = ConvertWgetSize(p);
					SetTotalSize(tot_size);
					p = strstr(line, "(");
					if (p != NULL) {
						p++;
						uint32 remains = ConvertWgetSize(p);
						SetReceivedSize(tot_size - remains);
					} else
						SetReceivedSize(0);
				}
				if (strstr(line, "K ->") != NULL) {
					SetState(T_RECEIVING);
					goto recv;
				}
				//for http
				if (strstr(line, "ERROR") != NULL) {
					SetState(T_NOTFOUND);
					//beep();
					//(new BAlert("","File not found","OK"))->Go();
				}
				// for ftp
				if (strncmp(line, "No such file ", 13) == 0) {
					SetState(T_NOTFOUND);
					//beep();
					//(new BAlert("","File not found","OK"))->Go();
				}
				// for ftp login
				if (strstr(line, "Login incorrect.") != NULL) {
					SetState(T_NOTFOUND);
				}
				// for auth
				if (strstr(line, "Authorization failed") != NULL) {
					SetState(T_NOTFOUND);
					BString title = "Authorization:<";
					int32 index = fURL.FindFirst("http://");
					int32 offset = 7;
					if (index == B_ERROR) {
						index = fURL.FindFirst("ftp://");
						offset = 6;
					}
					if (index != B_ERROR) {
						index += offset;
						int32 len = fURL.Length();
						for (int32 i = index; i < len ; i++) {
							if (fURL[i] == '/')
								break;
							title += fURL[i];
						}
					}
					title += ">";

					PasswordWindow* win = new PasswordWindow(
						RectUtils().CenterRect(270, 110),
						title.String(), fLogin.String(),
						fPassword.String(), this);
					win->Show();
				}
				break;
			}
		case T_RECEIVING: {
recv:
				int len = strlen(line);
				for (int i = 0; i < len; i++) {
					switch (line[i]) {
						case '.':
							SetReceivedSize(1024);
							break;
					}
				}
				if (strstr(line, "[100%]") != NULL) {
					SetReceivedSize(fTotalSize);
					SetState(T_FINISHED);
					//StopWatching();
					fWGetThread = -1;
				}
				break;
			}
	}
}

/***********************************************************
 * wget
 ***********************************************************/
void
HListItem::Wget() {
	int args = 1;
	SetState(T_CONNECTING);
	char* argv[40];

	argv[0] = strdup("wget");

	const char* option;
	((HApp*)be_app)->Prefs()->GetData("option", &option);

	BString line;
	int32 len = strlen(option);
	for (int32 i = 0; i < len; i++) {
		if (option[i] == ' ' && line.Length() != 0) {
			argv[args++] = strdup(line.String());
			line = "";
		} else if (option[i] != ' ')
			line += option[i];
	}
	if (line.Length() != 0)
		argv[args++] = strdup(line.String());

	if (fLogin.Length() != 0) {
		BString tmp = "--http-user=";
		tmp += fLogin;
		argv[args++] = strdup(tmp.String());
	}

	if (fPassword.Length() != 0) {
		BString tmp = "--http-passwd=";
		tmp += fPassword;
		argv[args++] = strdup(tmp.String());
	}

	argv[args++] = strdup(fURL.String());
	argv[args++] = NULL;
	int in, out;
	fWGetThread = pipe_command(args - 1, argv, in, out, fProcess);

	if (fWGetThread < B_NO_ERROR)
		(new BAlert("", "Could not find wget command", "OK"))->Go();
	else
		::resume_thread(fWGetThread);
	for (int i = 0 ; i < args; i++)
		free(argv[i]);
	close(in);
	close(out);
	fOldSize = 0;
	fStartTime = time(NULL);
	fReceivedSize = 0;
}

/***********************************************************
 * Convert wget size
 ***********************************************************/
uint32
HListItem::ConvertWgetSize(char* size) {
	char* p = size;

	while (*p != ' ') {
		if (*p == ',') {
			while (*p != ' ')
				*p++ = *(p + 1);
			p = size;
		} else
			p++;
	}
	return atol(size);
}

/***********************************************************
 * MoveToTrash
 ***********************************************************/
void
HListItem::MoveToTrash() {
	bool tracker_rule;

	((HApp*)be_app)->Prefs()->GetData("tracker_rule", &tracker_rule);

	if (!tracker_rule)
		return;
	// Check file exist
	BNode node(fLocalPath.String());
	if (node.InitCheck() != B_OK)
		return;
	if (!fForceDelete) {
		off_t size;
		node.GetSize(&size);
		if (size < fTotalSize)
			return;
	}
	// Send move to trash message to tracker
	BEntry entry;
	entry_ref ref;
	entry.SetTo(fLocalPath.String());
	entry.GetRef(&ref);

	/*   BMessenger tracker("application/x-vnd.Be-TRAK" );
	    BMessage msg( B_DELETE_PROPERTY ) ;

		BMessage specifier( 'sref' ) ;
		specifier.AddRef( "refs", &ref ) ;
		specifier.AddString( "property", "Entry" ) ;
		msg.AddSpecifier( &specifier ) ;

		BMessage reply ;
		tracker.SendMessage( &msg, &reply );
	*/
	TrackerUtils().MoveToTrash(ref);
	//::watch_node(&nref,B_WATCH_NAME,this);
}

/***********************************************************
 * RegistNodeMonitor
 ***********************************************************/
void
HListItem::RegistNodeMonitor() {
	bool tracker_rule;

	((HApp*)be_app)->Prefs()->GetData("tracker_rule", &tracker_rule);

	if (!tracker_rule)
		return;

	BFile file(fLocalPath.String(), B_CREATE_FILE | B_READ_ONLY);

	file.GetNodeRef(&fFileNodeRef);

	entry_ref ref;
	::get_ref_for_path(fLocalPath.String(), &ref);
	BMessage msg(M_ADD_WATCH_NODE);
	msg.AddRef("refs", &ref);

	((HApp*)be_app)->Window()->PostMessage(&msg);
}

/***********************************************************
 * FormatTime
 ***********************************************************/
void
HListItem::FormatTime(time_t& timet, BString& str) {
	char buf[10];
	int32 hour = 0, min = 0, sec = 0;
	if (timet < 0)
		timet = 0;
	if (timet > 0) {
		hour = timet / 3600;
		min = timet / 60;
		sec = timet % 60;
	}
	::sprintf(buf, "%.2ld:%.2ld:%.2ld", hour, min, sec);
	str = buf;
}

/***********************************************************
 * SetLoginAndPassword
 ***********************************************************/
void
HListItem::SetLoginAndPassword(const char* login, const char* password) {
	fLogin = login;
	fPassword = password;
}

/***********************************************************
 * Pipe command
 ***********************************************************/
thread_id
HListItem::pipe_command(int argc, char** argv, int& in, int& out, int& err,
						char** envp) {
	// Save current FDs
	int old_in = dup(0);
	int old_out = dup(1);
	int old_err = dup(2);

	int filedes[2];

	// Create new pipe FDs as stdin, stdout, stderr
	pipe(filedes); dup2(filedes[0], 0); close(filedes[0]);
	in = filedes[1]; // Write to in, appears on cmd's stdin
	pipe(filedes); dup2(filedes[1], 1); close(filedes[1]);
	out = filedes[0]; // Read from out, taken from cmd's stdout
	pipe(filedes); dup2(filedes[1], 2); close(filedes[1]);
	err = filedes[0]; // Read from err, taken from cmd's stderr


	// "load" command.
	thread_id ret = load_image(argc, (const char**)argv, (const char**)envp);
	// thread ret is now suspended.

	// Restore old FDs
	close(0); dup(old_in); close(old_in);
	close(1); dup(old_out); close(old_out);
	close(2); dup(old_err); close(old_err);
	return ret;
}

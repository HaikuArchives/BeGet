#ifndef __HLISTITEM_H__
#define __HLISTITEM_H__

#include "CLVEasyItem.h"
#include <String.h>
#include <stdio.h>
#include <stdlib.h>
#include <KernelKit.h>
#include <Node.h>

enum States {
	T_NOT_CONNECTED = 0,
	T_CONNECTED,
	T_CONNECTING,
	T_NOTFOUND,
	T_RECEIVING,
	T_FINISHED,
	T_SUSPEND
};

enum {
	M_DELETE_ITEM = 'DLIE'
};

class HListItem : public CLVEasyItem {
public:
	HListItem(const char* url
			  , const char* path = NULL
								   , uint32 size = 0);
	virtual 		~HListItem();

	const char*		Log() const;
	const char*		URL() const;
	const char*		FilePath() const {return fLocalPath.String();}
	void	SetDirty(bool dirty) { fDirty = dirty;}
	bool	IsDirty() const { return fDirty; }

	bool	IsSuspendable();
	void	Suspend();
	bool	IsResumable();
	void	Resume();
	void	Stop();
	bool	IsStarted();
	void	Start();
	States	State()const {return (States)fState;}
	void	RefreshTime();
	node_ref	NodeRef() const {return fFileNodeRef;}
	uint32	TotalSize()const {return fTotalSize;}
	void	SetForceDelete(bool del) {fForceDelete = del;}
	void	SetLoginAndPassword(const char* login, const char* password);
protected:
	static	int32 	Watcher_thread(void* data);
	void	Watcher();
	void	Wget();
	void	StartWatching();
	void	StopWatching();
	void	SetName(const char* name);
	void	SetTotalSize(uint32 size);
	void	SetReceivedSize(uint32 size);
	thread_id	pipe_command(int argc, char** argv,
							 int& in, int& out, int& err,
							 char** envp = environ);
	void	ProcessLogLine(const char* line);
	uint32	ConvertWgetSize(char* size);
	void	SetState(States state);
	void	CheckSize();
	void	MoveToTrash();
	void	RegistNodeMonitor();
	void	FormatTime(time_t& t , BString& out);

private:
	time_t		fElapsed;
	uint32		fTotalSize;
	uint32		fReceivedSize;
	BString		fURL;
	int			fProcess;
	BString		fLog;
	thread_id	fWatcherThread;
	thread_id	fWGetThread;
	bool		fContinueWatching;
	int32		fState;
	bool		fDirty;
	int32		fOldSize;
	time_t		fStartTime;
	BString		fLocalPath;
	int32		fOldState;
	bool		fForceDelete;
	node_ref	fFileNodeRef;
	BString		fLogin;
	BString		fPassword;
};
#endif

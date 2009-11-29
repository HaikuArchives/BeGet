#ifndef __HWINDOW_H__
#define __HWINDOW_H__

#include <Window.h>

class SplitPane;
class HLogView;
class HListView;
class URLSetting;

enum {
	M_ADD_URL = 'MAUD',
	M_SUSPEND = 'MSUS',
	M_RESUME = 'MRES',
	M_STOP = 'MSTP',
	M_START = 'MSAR',
	M_DELETE = 'MDEL',
	M_PREFS = 'MPRF',
	M_DELETE_FINISHED = 'MDFN',
	M_OPEN_FOLDER = 'MoPF'
};

class HWindow : public BWindow {
public:
	HWindow(BRect rect , const char* name);
protected:
	virtual			~HWindow();
	virtual	bool	QuitRequested();
	virtual void	MenusBeginning();
	virtual void	MessageReceived(BMessage* message);
	virtual void	DispatchMessage(BMessage* message, BHandler* target);
	void	InitGUI();
	void	InitMenu();
	void	Pulse();
	void	OpenAddUrlDlg(const char* url);
	void	WhenDropped(BMessage* msg);
	void	Watch();
	bool	CheckExt(const char* url, const char* ext);
	void	InstallToDeskbar();
private:
	SplitPane*		fHSplitter;
	HLogView*		fLogView;
	HListView*		fListView;
	URLSetting*		fURLSetting;

};
#endif
#ifndef __HAPP_H__
#define __HAPP_H__

#include "LocaleApp.h"
#include <SimpleGameSound.h>
#include "HPrefs.h"

#define APP_SIG "application/x-vnd.takamatsu-beget"

class HWindow;
class SettingsFile;

enum {
	M_SOUND_DOWNLOAD = 'SODM',
	M_SET_DOWNLOAD_SOUND = 'MSED',
	M_ADD_WATCH_NODE = 'MADN'
};

class HApp : public LocaleApp {
public:
	HApp();
	virtual			~HApp();

	HPrefs*			Prefs() {return fPrefs;}
	HWindow*		Window() {return fWindow;}
	void	SetSound(const char* path);
protected:
	virtual void	AboutRequested();
	virtual void	MessageReceived(BMessage* message);
	virtual void	RefsReceived(BMessage* message);
private:
	HWindow*		fWindow;
	HPrefs*			fPrefs;
	BSimpleGameSound* fGameSound;
	typedef	LocaleApp	_inherited;
};
#endif
#include "HApp.h"
#include "HWindow.h"
#include "HAboutWindow.h"

#include <Alert.h>
#include <string.h>

/***********************************************************
 * Constructor
 ***********************************************************/
HApp::HApp() :_inherited(APP_SIG)
	,fGameSound(NULL)
{
	fPrefs = new HPrefs("BeGet.prefs");
	fPrefs->LoadPrefs();
	BRect rect;
	
	fPrefs->GetData("main_window",&rect);
	
	fWindow = new HWindow(rect,"BeGet");
	fWindow->Show();
	
	const char* path;
	fPrefs->GetData("snd_download",&path);
	SetSound(path);
}	

/***********************************************************
 * Destructor
 ***********************************************************/
HApp::~HApp()
{
	delete fPrefs;
	delete fGameSound;
}

/***********************************************************
 * MessageReceived
 ***********************************************************/
void
HApp::MessageReceived(BMessage *message)
{
	switch(message->what)
	{
	case M_ADD_URL:
	case M_OPEN_FOLDER:
		fWindow->PostMessage(message);
		break;
	case M_SOUND_DOWNLOAD:
		if(fGameSound)
			fGameSound->StartPlaying();
		break;
	case M_SET_DOWNLOAD_SOUND:
		{
			const char *path;
			if(message->FindString("path",&path) == B_OK)
				SetSound(path);
			break;
		}
	default:
		_inherited::MessageReceived(message);
	}
}

/***********************************************************
 * SetSound
 ***********************************************************/
void
HApp::SetSound(const char* path)
{
	if(fGameSound)
		delete fGameSound;
	fGameSound = NULL;
	
	if( ::strlen(path) > 0)
	{
		fGameSound = new BSimpleGameSound(path);
		if(fGameSound->InitCheck() != B_OK)
		{
			fPrefs->SetData("snd_download","");
			delete fGameSound;
			fGameSound = NULL;
			return;
		}
		fGameSound->SetIsLooping(false);
	}
}

/***********************************************************
 *
 ***********************************************************/
void
HApp::RefsReceived(BMessage *message)
{
	const char *url;
	if(message->FindString("be:url",&url) == B_OK)
		fWindow->PostMessage(message);	
}


/***********************************************************
 * AboutRequested
 ***********************************************************/
void
HApp::AboutRequested()
{
	(new HAboutWindow("BeGet",
					__DATE__,
					"Created by Atsushi Takamatsu@Sapporo,Japan",
					"http://beget.sourceforge.net",
					"atsushi@io.ocn.ne.jp"))->Show();
}
#ifndef __HSETTING_H__
#define __HSETTING_H__
/*********** SYSTEM HEADER ***********/
#include <Locker.h>
#include <SupportDefs.h>
#include <GraphicsDefs.h>
#include <StorageDefs.h>
#include <Message.h>
#include <Path.h>
#include <Directory.h>
#include <Entry.h>
#include <File.h>
#include <Alert.h>
#include <stdio.h>
#include <String.h>
#include <FindDirectory.h>

const char	K_PREFS_FILE_NAME[] = "K_DEFAULT_FILE";

class HSetting:public BLocker
{
public:
			HSetting(const char* name = K_PREFS_FILE_NAME
					, const char* dir_name = NULL);
	virtual		~HSetting();
	
	void	LoadPrefs();
	void	StorePrefs();
	
	
	void	GetData(const char* name, BRect* rect);
	void	GetData(const char* name, BPoint* point);
	void	GetData(const char* name, const char** text);
	void	GetData(const char* name, int32* num);
	void	GetData(const char* name, int16* num);
	void	GetData(const char* name, int8* num);
	void	GetData(const char* name, bool* status);
	void	GetData(const char* name, float* num);
	void	GetData(const char* name, entry_ref* ref);
	void GetData(const char* name, rgb_color *color);
	void	GetData(const char* name, void** data);
	void	GetData(const char* name, void** data, ssize_t* numBytes);
	void GetData(const char* name,type_code type,void* color,ssize_t *numBytes);
	
	void	SetData(const char* name, BRect rect);
	void	SetData(const char* name, BPoint point);
	void	SetData(const char* name, const char* text);
	void	SetData(const char* name, int32 num);
	void	SetData(const char* name, int16 num);
	void	SetData(const char* name, int8 num);
	void	SetData(const char* name, bool status);
	void	SetData(const char* name, float num);
	void	SetData(const char* name, entry_ref* ref);
	void	SetData(const char* name, rgb_color color);
	void	SetData(const char* name, const void* data);
	void	SetData(const char* name, const void* data, ssize_t numBytes);
	void SetData(const char* name, type_code type ,const void* data, ssize_t numBytes);
	
	void	PrintToStream();
	
protected:
	virtual void	MakeDefault();
	void			SetDefaultData(const char* name
									, const void* data
									, ssize_t numBytes);
	BMessage*	fDefaultMessage;
private:
	BEntry*		fPrefsEntry;
	BMessage*	fSettingMessage;
	BString		fDir_name;
	BString		fName;
	
};

#endif








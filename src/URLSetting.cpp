#include "URLSetting.h"
#include "HApp.h"

#include <Path.h>
#include <Directory.h>
#include <FindDirectory.h>
#include <File.h>
#include <Message.h>
#include <Alert.h>
#include <stdlib.h>
#include <iostream>
#include <String.h>

/***********************************************************
 * Constructor
 ***********************************************************/
URLSetting::URLSetting()
	:BMessage(B_SIMPLE_DATA)
{
	BPath		path;
	BDirectory	dir;

	
	const char* name = "BeGet-URL.pref";
	
	::find_directory(B_USER_SETTINGS_DIRECTORY, &path, true);
	dir.SetTo(path.Path());
		
	if(dir.FindEntry(name, &fEntry) == B_NO_ERROR)
	{	
	
	}else{
		BFile	file;
		if(dir.CreateFile(name, &file) == B_NO_ERROR)
		{
			dir.FindEntry(name, &fEntry);	
		}else{
			(new BAlert("",_("Could not create setting file."),_("OK")))->Go();
		}
	}
	
	BFile setting(&fEntry,B_READ_WRITE);
	Unflatten(&setting);
	
}
/***********************************************************
 * Destructor
 ***********************************************************/
URLSetting::~URLSetting()
{
	BFile file(&fEntry,B_WRITE_ONLY|B_CREATE_FILE|B_ERASE_FILE);
	if(file.InitCheck() == B_OK)
	{
		Flatten(&file);
	}
}

/***********************************************************
 * Add urls
 ***********************************************************/
void
URLSetting::AddURL(const char* url,const char* path,uint32 size)
{
	AddString("url",url);
	AddInt32("size",size);
	AddString("path",path);
}

/***********************************************************
 * CountItems
 ***********************************************************/
int32
URLSetting::CountItems() const
{
	int32 count = 0;
	type_code type;
	
	if(GetInfo("url",&type,&count) != B_OK)
		count =0;
	
	return count;
}

/***********************************************************
 * FindURL
 ***********************************************************/
const char*
URLSetting::FindURL(int32 index)
{
	return FindString("url",index);
}

/***********************************************************
 * FindPath
 ***********************************************************/
const char*
URLSetting::FindPath(int32 index)
{
	return FindString("path",index);
}

/***********************************************************
 * FindSize
 ***********************************************************/
uint32
URLSetting::FindSize(int32 index)
{
	return FindInt32("size",index);
}
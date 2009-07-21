#include "HPrefs.h"
#include <Beep.h>
#include <Autolock.h>

HPrefs::HPrefs(const char* name, const char* dir_name)
	: HSetting(name,dir_name)
{
	MakeDefault();
}


HPrefs::~HPrefs()
{
}

void
HPrefs::MakeDefault()
{
	BAutolock lock(this);
	
	fDefaultMessage->AddRect("main_window", BRect(50,50,500,400));
	fDefaultMessage->AddInt32("hbar_pos", 150);
	fDefaultMessage->AddInt16("col1",100);
	fDefaultMessage->AddInt16("col2",100);
	fDefaultMessage->AddInt16("col3",100);
	fDefaultMessage->AddInt16("col4",100);
	fDefaultMessage->AddInt16("col5",100);
	fDefaultMessage->AddInt16("col6",100);
	fDefaultMessage->AddBool("auto_start",true);
	fDefaultMessage->AddBool("auto_launch",false);
	fDefaultMessage->AddString("option","-c -P /boot/home/Downloads");
	fDefaultMessage->AddBool("auto_delete",false);
	fDefaultMessage->AddBool("watch",true);
	fDefaultMessage->AddBool("watch_confirm",true);
	fDefaultMessage->AddString("watch_ext","zip gz");
	fDefaultMessage->AddBool("deskbar",true);
	fDefaultMessage->AddString("snd_download","");
	fDefaultMessage->AddBool("tracker_rule",false);
	fDefaultMessage->AddBool("queue",true);
	fDefaultMessage->AddInt32("max_transfer",3);
	_inherited::MakeDefault();
}
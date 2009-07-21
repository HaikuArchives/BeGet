#ifndef __OPENWITHMENU_H__
#define __OPENWITHMENU_H__

#include <Menu.h>

enum{
	M_OPEN_WITH_MSG = 'OWMU'
};

class OpenWithMenu :public BMenu{
public:
						OpenWithMenu(const char* name,const char* mime_type);
	virtual				~OpenWithMenu();
			void		ClearItems();
protected:
	virtual bool 		AddDynamicItem(add_state state);
			bool		BuildItems();
			bool		AddNextItem();	
private:
	static	int32		StartBuilding(void *data);
	bool				fCancel;
	bool				fMenuBuilt;
	thread_id			fThread;
	BList				fItemList;
};
#endif
#ifndef __PASSWIN_H__
#define __PASSWIN_H__

#include <Window.h>

class HListItem;

enum {
	M_PASS_MSG = 'MPAS'
};

class PasswordWindow : public BWindow {
public:
	PasswordWindow(BRect rect
				   , const char* name
				   , const char* login
				   , const char* pass
				   , HListItem* item);
protected:
	virtual			~PasswordWindow();
	virtual void	MessageReceived(BMessage* message);


private:
	HListItem*		fListItem;

};
#endif
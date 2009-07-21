/*************************************************************
*   Toolbar©
*
*   Toolbar is a usefull UI component.
*
*   @author  Atsushi Takamatsu (tak_atsu@tau.bekkoame.ne.jp)
**************************************************************/
#ifndef __HTOOLTIPSWINDOW_H__
#define __HTOOLTIPSWINDOW_H__


#include <Window.h>
#include <Message.h>

class HToolTipsWindow :public BWindow {
public:
					HToolTipsWindow(BView *owner,BPoint where,const char* tips);
					~HToolTipsWindow();
			void	SetEnabled(bool enabled);
protected:
	virtual void	DispatchMessage(BMessage *message,BHandler *target);
private:
	BView			*fOwner;
	int				fCurrentDelay;
};
#endif
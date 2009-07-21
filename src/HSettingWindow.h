#ifndef __HPREFERENCE_WINDOW_H__
#define __HPREFERENCE_WINDOW_H__

#include <Window.h>

class HWGetSettingView;
class HBeGetSettingView;
class HSoundSettingView;

enum{
	M_APPLY_MESSAGE = 'MAPL',
	M_SETTING_CHANGED = 'MSTC',
	M_SET_CHAT_FONT_MSG = 'MSEF'
};

class HSettingWindow :public BWindow {
public:
				HSettingWindow(BRect rect);
	virtual		~HSettingWindow();

protected:
		void	InitGUI();
virtual void	MessageReceived(BMessage *message);
virtual bool	QuitRequested();

private:
		HWGetSettingView *wgetsetting;
		HBeGetSettingView* begetsetting;
		HSoundSettingView*	soundsetting;

};
#endif
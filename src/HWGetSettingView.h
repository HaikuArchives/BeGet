#ifndef __HOTHERSETTING_VIEW_H__
#define __HOTHERSETTING_VIEW_H__

#include <View.h>

class CTextView;

class HWGetSettingView: public BView {
public:	
				HWGetSettingView(BRect rect);
	virtual		~HWGetSettingView();
		void	InitGUI();
	const char*	Option()const;
protected:
	virtual void	MessageReceived(BMessage *message);
private:
	CTextView*	fOptionView;
};
#endif
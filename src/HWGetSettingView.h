#ifndef __HOTHERSETTING_VIEW_H__
#define __HOTHERSETTING_VIEW_H__

#include <View.h>
#include <TextView.h>

class HWGetSettingView: public BView {
public:
	HWGetSettingView(BRect rect);
	virtual		~HWGetSettingView();
	void	InitGUI();
	const char*	Option()const;
protected:
	virtual void	MessageReceived(BMessage* message);
private:
	BTextView*	fOptionView;
};
#endif

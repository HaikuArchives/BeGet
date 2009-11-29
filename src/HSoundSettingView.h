#ifndef __HSOUNDSETTING_VIEW_H__
#define __HSOUNDSETTING_VIEW_H__

#include <View.h>
#include <MenuField.h>
#include <FilePanel.h>

enum {
	M_ITEM_MESSAGE = 'MIEM',
	M_OTHER_MESSAGE = 'MOTH',
	M_NONE_MESSAGE = 'NONM'
};

class HSoundSettingView: public BView {
public:
	HSoundSettingView(BRect rect);
	virtual		~HSoundSettingView();
	void	InitGUI();
	const char*	DownloadSound();
protected:
	virtual void	MessageReceived(BMessage* message);
	void	SetupMenuField();
private:
	BMenuField*		fMenuField;
	BFilePanel*		fFilePanel;
	int32			fOldItem;
};
#endif
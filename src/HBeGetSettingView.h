#ifndef __HBEGETSETTING_VIEW_H__
#define __HBEGETSETTING_VIEW_H__

#include <View.h>

enum{
	M_WATCH_CHANGED = 'MCHD',
	M_DELETE_CHANGED = 'MDEL',
	M_QUEUE_CAHNGED = 'mQUC'
};

class HBeGetSettingView: public BView {
public:	
				HBeGetSettingView(BRect rect);
	virtual		~HBeGetSettingView();
		void	InitGUI();
		bool	Launch();
		bool	AutoStart();
		bool	AutoDelete();
		bool	Watch();
		bool	Confirm();
		bool	Deskbar();
		bool	TrackerRule();
		int32	MaxTransfers();
		bool	Queue();
	const char*	Ext();
protected:
	virtual void	MessageReceived(BMessage *message);
private:
	BTextControl	*fExtensions;
};
#endif
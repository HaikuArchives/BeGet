#ifndef __HTASKVIEW_H__
#define __HTASKVIEW_H__

#include <View.h>

enum{
	M_SHOW_MSG = 'MSHO'
};

class _EXPORT HTaskView :public BView {
public:
						HTaskView(BRect frame);
						HTaskView(BMessage *data);
		virtual			~HTaskView();
	
protected:	
		virtual void	Draw(BRect updateRect);
		// archiving overrides
		static 	HTaskView *Instantiate(BMessage *data);
		virtual	status_t Archive(BMessage *data, bool deep = true) const;
		virtual 	void MouseDown(BPoint);
		virtual void	MessageReceived(BMessage *message);
				void	WhenDropped(BMessage* message);

private:
		BBitmap 		*fIcon;
		char			*fLabels[3];
};
#endif
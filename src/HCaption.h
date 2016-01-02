#ifndef __HCaption_H__
#define __HCaption_H__
#include <String.h>
#include <View.h>
#include <StringView.h>
#include <ListView.h>
#include <iostream>

#include "HListView.h"

class HCaption : public BView {
public:
	HCaption(BRect rect,
			 const char* name,
			 HListView* target = NULL);
	virtual			~HCaption();

protected:
	void	SetCaption(int32 num);
	virtual void 	Pulse();
private:
	BStringView*	 view;
	HListView*		 fTarget;
	int32			fOld;
};
#endif

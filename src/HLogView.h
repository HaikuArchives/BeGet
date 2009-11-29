#ifndef __HLOGVIEW_H__
#define __HLOGVIEW_H__

#include "CTextView.h"

class HLogView : public CTextView {
public:
	HLogView(BRect rect,
			 const char* name,
			 int32 resize,
			 int32 flags);
	virtual			~HLogView();
	void	ScrollToEnd();
protected:

private:
	typedef CTextView _inherited;
};
#endif
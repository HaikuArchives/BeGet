#ifndef __HLOGVIEW_H__
#define __HLOGVIEW_H__

#include <TextView.h>

class HLogView : public BTextView {
public:
	HLogView(BRect rect,
			 const char* name,
			 int32 resize,
			 int32 flags);
	virtual			~HLogView();
	void	ScrollToEnd();
protected:

private:
	typedef BTextView _inherited;
};
#endif

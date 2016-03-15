#ifndef __HLOGVIEW_H__
#define __HLOGVIEW_H__

#include <TextView.h>

class HLogView : public BTextView {
public:
	HLogView(const char* name,
			 int32 flags);
	virtual			~HLogView();
	void	ScrollToEnd();
protected:

private:
	typedef BTextView _inherited;
};
#endif

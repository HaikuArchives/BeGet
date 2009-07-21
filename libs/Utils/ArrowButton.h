#ifndef __ARROWBUTTON_H__
#define __ARROWBUTTON_H__

#include <View.h>
#include <Bitmap.h>

class ArrowButton :public BView{
public:
					ArrowButton(BRect rect,
								const char* name,
								BMessage *message,
								uint32 resize = B_FOLLOW_TOP|B_FOLLOW_LEFT,
								uint32 flag = B_WILL_DRAW);
	virtual			~ArrowButton();
			void	SetState(int32 state);
			int32	State() const {return fState;}
protected:
	virtual void	Draw(BRect updateRect);
	virtual void	MouseDown(BPoint pos);
private:
	BBitmap			 *fRightBitmap;
	BBitmap			*fDownBitmap;
	int32 			fState;
	BMessage		*fMessage;
};
#endif
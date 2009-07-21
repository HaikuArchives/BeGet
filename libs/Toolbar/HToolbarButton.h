/*************************************************************
*   Toolbar©
*
*   Toolbar is a usefull UI component.
*
*   @author  Atsushi Takamatsu (tak_atsu@tau.bekkoame.ne.jp)
**************************************************************/
#ifndef __HTOOLBAR_BUTTON_H__
#define __BTOOLBAR_BUTTON_H__

#include <View.h>
#include <String.h>
#include <Picture.h>
#include <Message.h>
#include <Bitmap.h>

#include <string>
#include "HToolTipsWindow.h"

class HToolbarButton :public BView {
public:
					HToolbarButton(BRect rect,const char* name,BBitmap *bitmap,BMessage *msg,const char *tips = NULL);
					~HToolbarButton();
				
				void InitPictures();
				
				/****** Enable & disable buttons *****/
				void SetEnabled(bool enable);
protected:
		virtual void MouseMoved(BPoint point,uint32 transit,const BMessage *message);
		virtual void MouseDown(const BPoint point);
		virtual void Draw(BRect rect);
		virtual void MouseUp(const BPoint point);
		virtual void Pulse();
				void DrawString(BView *view ,const char* label,bool downState = false,bool enbaled = true);
				void DrawBitmap(BView *view,BBitmap *bitmap,bool downState = false);
		/******* Make outsize state picture *******/
		BPicture*   MakeOutsidePicture(BBitmap *bitmap);
		/******* Make inside state picture *******/
		BPicture*   MakeInsidePicture(BBitmap *bitmap);
		/******* Make down state picture *******/
		BPicture*   MakeDownPicture(BBitmap *bitmap);
		/******* Make disable state picture *******/
		BPicture*	MakeDisablePicture(BBitmap *in);
private:
		BPicture*	fOutsidePicture;
		BPicture*	fInsidePicture;
		BPicture*	fDownPicture;
		BPicture*	fDisablePicture;
		BPicture*	fPicture;
		HToolTipsWindow* fTipsWindow;
		BString		fTips;
		bool		fUpdate;
		bool		fEnabled;
		BString		fName;
		uint8		fState;
		BMessage*	fMsg;
		BBitmap		*fBitmap;
};
#endif
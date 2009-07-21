/*************************************************************
*   Toolbar©
*
*   Toolbar is a usefull UI component.
*
*   @author  Atsushi Takamatsu (tak_atsu@tau.bekkoame.ne.jp)
**************************************************************/
#ifndef __HTOOLBAR_H__
#define __HTOOLBAR_H__

#include <Box.h>

/*
 * This message will send to parent window to update toolbar butotns.
 * it have button name and button pointer.
 */
#define M_UPDATE_TOOLBUTTON 'MUTB'
#define NORMAL_WIDTH 22
#define WITH_LABEL_WIDTH 40

class HToolbar : public BBox {
public:
				HToolbar(BRect rect,
						uint32 resize=B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP);
virtual			~HToolbar();
		/******* Add separatro bar. ******/
		void	AddSpace();
		/******* Add button with bitmap data ******/
		void	AddButton(const char* name,const void* icon,color_space space,BMessage *msg,const char* tips = NULL);
		/******* Add button with BBitmap ******/
		void	AddButton(const char* name,BBitmap *bitmap,BMessage *msg,const char* tips = NULL);

		void	UseLabel(bool use) {fUseLabel = use;}
		BRect	ButtonRect();
protected:


private:
		/******* Number of buttons ******/
		uint32 fButtons;
		/******* Number of separators *****/
		uint32 fSpaces;
		bool	fUseLabel;
}; 
#endif
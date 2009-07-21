#ifndef __MenuUtils_H__
#define __MenuUtils_H__

#include <Menu.h>
#include <MenuItem.h>

class MenuUtils
{
	public:
		MenuUtils(void);
	
		void	AddFontMenu(BMenu* menu
						,uint32 what 
						,const BHandler*	handler = NULL
						,const BLooper* 	looper = NULL
						,bool render_family = true);
		void	DeMarkAll(BMenu* menu
						,int32 fromIndex = 0
						,int32 toIndex = 0);
		void	AddFontFamilyMenu(BMenu *menu,uint32 what
						,const BHandler* handler = NULL
						,const BLooper*	looper = NULL);
		void	AddFontStyleMenu(BMenu* menu,font_family,uint32 what
						,const BHandler* handler = NULL
						,const BLooper* looper = NULL);
		void	AddMenuItem(BMenu* menu
							, const char* name
							, const uint32 what
							, const BHandler* handler
							, const BLooper* looper = NULL
							, char shortcut = 0
							, uint32 modifiers = 0
							, BBitmap *bitmap = NULL);
		void	AddMenuItem(BMenu* menu
							, const char* name
							, BMessage *message
							, const BHandler* handler
							, const BLooper* looper = NULL
							, char shortcut = 0
							, uint32 modifiers = 0
							, BBitmap *bitmap = NULL);
		void	AddMenuItem(BMenu* menu
							, BMenu* submenu
							, BMessage *message
							, const BHandler* handler
							, const BLooper* looper = NULL
							, char shortcut = 0
							, uint32 modifiers = 0
							, BBitmap *bitmap = NULL);
		void	AddMenuItem(BMenu* menu
							, BMenu* submenu
							, const uint32 what
							, const BHandler* handler
							, const BLooper* looper = NULL
							, char shortcut = 0
							, uint32 modifiers = 0
							, BBitmap *bitmap = NULL);
		void	SetFamilyAndStyle(BMenu* font_menu
							,const char* family
							,const char* style);
		
};
#endif


















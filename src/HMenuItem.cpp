#include "HMenuItem.h"

/***********************************************************
 * Constructor
 ***********************************************************/
HMenuItem::HMenuItem(	const char *label,
						BMessage *message,
						char shortcut,
						uint32 modifiers)
		:BMenuItem(label,message,shortcut,modifiers)
		,fPath("")
{

}

/***********************************************************
 * Destructor
 ***********************************************************/
HMenuItem::~HMenuItem()
{
}
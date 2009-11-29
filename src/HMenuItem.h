#ifndef __HMENUITEM_H__
#define __HMENUITEM_H__

#include <MenuItem.h>
#include <String.h>

class HMenuItem : public BMenuItem {
public:
	HMenuItem(const char* label,
			  BMessage* message,
			  char shortcut = 0,
			  uint32 modifiers = 0);
	virtual		~HMenuItem();

	void	SetPath(const char* path) {fPath = path;}
	const char* Path() const {return fPath.String();}
private:
	BString		fPath;
};
#endif
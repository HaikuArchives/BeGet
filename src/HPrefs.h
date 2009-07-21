#ifndef __HPREF_H__
#define __HPREF_H__

#include "HSetting.h"

class HPrefs:public HSetting
{
public:
			HPrefs(const char* name = K_PREFS_FILE_NAME
					, const char* dir_name = NULL);
	virtual		~HPrefs();

protected:
	typedef HSetting _inherited;
	virtual void	MakeDefault();
};

#endif








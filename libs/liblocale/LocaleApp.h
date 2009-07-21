#ifndef __LOCALEAPP_H__
#define __LOCALEAPP_H__

#include <Application.h>

class LocaleUtils;

#define _( String ) ((LocaleApp*)be_app)->GetText( String )

class LocaleApp :public BApplication {
public:
						LocaleApp(const char *signature);
	virtual				~LocaleApp();
	const char*			GetText(const char* text);
private:
	LocaleUtils			*fLocaleUtils;
};
#endif
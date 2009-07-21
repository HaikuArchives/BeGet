#ifndef __LOCALE_UTILS_H__
#define __LOCALE_UTILS_H__


class LocaleUtils  {
public:
						LocaleUtils(const char* app_sig);
	virtual 			~LocaleUtils();
	
	const char*			GetText(const char* text);
		
private:
			void		InitData(const char* lang,const char* app_sig);					
protected:
	BMessage*			fStrings;

};
#endif
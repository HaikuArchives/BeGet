#ifndef __URLSETTING_H__
#define __URLSETTING_H__

#include <Message.h>
#include <Entry.h>

class URLSetting :public BMessage {
public:
				URLSetting();
	virtual 	~URLSetting();
		int32	CountItems() const;
	
		void	AddURL(const char* url,const char* path,uint32 size);
		
		uint32  FindSize(int32 index);
	const char*	FindPath(int32 index);
	const char* FindURL(int32 index);
protected:

private:
	BEntry 	fEntry;
};
#endif
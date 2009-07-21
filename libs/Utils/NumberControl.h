#ifndef __NUMBERCONTROL_H__
#define __NUMBERCONTROL_H__

#include <TextControl.h>
#include <MessageFilter.h>
#include <Message.h>
#include <Handler.h>
#include <InterfaceDefs.h>
#include <SupportDefs.h>

class NumberFilter;

class NumberControl :public BTextControl {
public:
						NumberControl(BRect frame,
								const char *name,
								const char *label, 
								int32 initial_number, 
								BMessage *message,
								uint32 rmask = B_FOLLOW_LEFT | B_FOLLOW_TOP,
								uint32 flags = B_WILL_DRAW | B_NAVIGABLE); 
			virtual 	~NumberControl();
					void SetValue(int32 value);
				int32	Value() const;
protected:											
			
private:
			NumberFilter		*filter;
};

class NumberFilter : public BMessageFilter {
public:
						NumberFilter();
virtual filter_result	Filter(BMessage *message, BHandler **target);

};
#endif
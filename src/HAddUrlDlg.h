#ifndef __HADDURLDLG_H__
#define __HADDURLDLG_H__

#include <Window.h>
#include <TextView.h>

class CTextView;

enum {
	M_OK_MESSAGE = 'MOKM',
	M_REAL_ADD = 'MREA'
};

class HAddUrlDlg : public BWindow {
public:
	HAddUrlDlg(BRect rect,
			   const char* url = NULL);
protected:
	virtual			~HAddUrlDlg();
	virtual void	MessageReceived(BMessage* message);
	void	InitGUI();
private:
	BTextView*		fURLView;

};
#endif

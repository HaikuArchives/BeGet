#ifndef __HLISTVIEW_H__
#define __HLISTVIEW_H__

#include <private/interface/ColumnListView.h>
#include <Node.h>

enum {
	M_SELECTION_CHANGED = 'MSEL',
	M_LIST_DBL_CLICKED = 'MLID'
};

class HListView : public BColumnListView {
public:
	HListView(const char* name);
	virtual 		~HListView();
	void	DeletePointers();
	void	DeleteItem(int32 index);
	void	AddURL(const char* url
				   , const char* path  = NULL
										 , uint32 size = 0);
	int32	FindNextSelection(int32 index);
	int32	CountDownloadingItems();
protected:
	virtual void	KeyDown(const char* bytes, int32 numBytes);
	virtual void	Pulse();
	virtual void	MouseDown(BPoint pos);
	virtual void	MessageReceived(BMessage* message);
	void	GoodbyeFile(node_ref nref);
private:
	BList	fPointerList;

};
#endif

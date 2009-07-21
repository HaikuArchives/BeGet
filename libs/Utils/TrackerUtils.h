#ifndef __TRACKERUTILS_H__
#define __TRACKERUTILS_H__

#include <Message.h>
#include <Entry.h>
#include <Messenger.h>

class TrackerUtils {
public:
					TrackerUtils();
	virtual			~TrackerUtils();
	
			void	MoveToTrash(entry_ref file_ref);
			void	OpenFolder(entry_ref folder_ref);
};
#endif
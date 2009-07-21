#ifndef _SPLIT_PANE_CONFIG_H
#define _SPLIT_PANE_CONFIG_H

#include <Application.h>
#include <AppKit.h>
#include <InterfaceKit.h>

#include "SplitPane.h"

class SplitPaneConfig : public BWindow {
   public:
      SplitPaneConfig(SplitPane*);
      virtual void MessageReceived(BMessage*);
      virtual bool QuitRequested();
   private:

};
#endif
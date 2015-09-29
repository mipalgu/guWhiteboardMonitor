/* MiPAL 2011
 Author: Carl Lusty
 */
#include "gusimplewhiteboard.h"
#include "guwhiteboardwatcher.h"

#pragma clang diagnostic ignored "-Wreserved-id-macro"

#ifdef USE_OLD_WHITEBOARD
#define FROM_WHITEBOARD_CC_    // shut up the whiteboard warning, I know what I'm doing

#include "Whiteboard.h"
#endif

class GUMonitor
{
public:
	GUMonitor(int rwb, char **subscription_list = NULL, int n = 0);
	
	~GUMonitor();
	
private:
        void callback(guWhiteboard::WBTypes t, gu_simple_message *msg);
        whiteboard_watcher *watcher;    /** new whiteboard object */
#ifdef USE_OLD_WHITEBOARD
        guWhiteboard::Whiteboard *wb;	/**< Whiteboard object. */
        guWhiteboard::Whiteboard::WBResult r; /**< Return message from wb methods. */
#endif
	pthread_mutex_t  sMutex; /**< Keep things thread safe, don't want two callbacks running at the same time. */
};

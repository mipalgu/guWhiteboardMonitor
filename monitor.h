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
	GUMonitor(const char *name, int rwb, char **subscription_list = NULLPTR, int n = 0, bool output_timestamps = false, bool output_script = false);
	
	~GUMonitor();
	
private:
        void callback(CPP_WHITEBOARD_NAMESPACE::WBTypes t, gu_simple_message *msg);
        whiteboard_watcher *watcher;    /** new whiteboard object */
	gu_simple_whiteboard_descriptor *wbd;
#ifdef USE_OLD_WHITEBOARD
        guWhiteboard::Whiteboard *wb;	/**< Whiteboard object. */
        guWhiteboard::Whiteboard::WBResult r; /**< Return message from wb methods. */
#endif
	pthread_mutex_t  sMutex; /**< Keep things thread safe, don't want two callbacks running at the same time. */
	long long timeStamp;	///< time stamp of previous call back
	bool printTimeStamps;	///< print time stamps?
	bool printScript;	///	< print as .sh script?
};

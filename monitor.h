/* MiPAL 2011
 Author: Carl Lusty
 */

#include "guwhiteboardwatcher.h"
#include "Whiteboard.h"


class GUMonitor
{
public:
	GUMonitor(char *whiteboardLocation = (char *)"", char **subscription_list = NULL, int n = 0);
	
	~GUMonitor();
	
private:
	void monitorCallback(std::string dataName, WBMsg *value);
        void callback(guWhiteboard::WBTypes t, gu_simple_message *msg);
        whiteboard_watcher *watcher;    /** new whiteboard object */
        guWhiteboard::Whiteboard *wb;	/**< Whiteboard object. */
        guWhiteboard::Whiteboard::WBResult r; /**< Return message from wb methods. */
	pthread_mutex_t  sMutex; /**< Keep things thread safe, don't want two callbacks running at the same time. */ 	
};

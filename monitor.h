/* MiPAL 2011
 Author: Carl Lusty
 */


#include "Whiteboard.h"


using namespace std;
using namespace guWhiteboard;

class GUMonitor
{
public:
	GUMonitor(char *whiteboardLocation = (char *)"", char **subscription_list = NULL, int n = 0);
	
	~GUMonitor();
	
private:
	void monitorCallback(std::string dataName, WBMsg *value);
	
	Whiteboard *wb;	/**< Whiteboard object. */ 
	Whiteboard::WBResult r; /**< Return message from wb methods. */ 
	pthread_mutex_t  sMutex; /**< Keep things thread safe, don't want two callbacks running at the same time. */ 	
};

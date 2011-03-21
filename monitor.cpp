#include "monitor.h"
#include <sstream>

int main(int argc, char **argv) 
{
	fprintf(stderr, "\n **** GU WHITEBOARD MONITOR MODULE ****\n (c) 2011 Carl Lusty\n\n");
	
	
	//Get passed in values
	//-----------------------------------
	int op;
	char *specAddressOfWB = (char *)"";
	
	while((op = getopt(argc, argv, "w:")) != -1)
	{
		switch(op)
		{
			case 'w':
				specAddressOfWB = optarg;
				break;
			case '?':			
				fprintf(stderr, "\n\nUsage: guWhiteboardMonitor [OPTION] . . . \n");
				fprintf(stderr, "-w, The name of the Whiteboard to connect to.\n");
				return EXIT_FAILURE;
				break;
			default:
				break;
		}
	}	
	//-----------------------------------
	
	//Start game
	GUMonitor *monitor = new GUMonitor(specAddressOfWB);
	
	//Currently waiting for events, loop to keep process from closing
	while(true)
	{
		sleep(5);
	}
	return 0;
}

GUMonitor::GUMonitor(char *whiteboardLocation)
{
	//Setup
	//----------------------------------
	if(strlen(whiteboardLocation) == 0)
	{
		wb = new Whiteboard();		
	}
	else
	{
		wb = new Whiteboard(whiteboardLocation);
	}
	pthread_mutex_init(&sMutex, NULL);
	//----------------------------------
	
	//Subscriptions
	//----------------------------------
	wb->subscribeToMessage("*", WB_BIND(GUMonitor::monitorCallback), r);
	if(r != Whiteboard::METHOD_OK)
	{
		fprintf(stderr, "Failed to subscribe\n");
	}
	
}

GUMonitor::~GUMonitor()
{
	
}



void GUMonitor::monitorCallback(std::string dataName, WBMsg *value)
{	
	pthread_mutex_lock (&sMutex);
	
	std::ostringstream out;
	switch ((int)value->getType()) 
	{
		case 0:
		{
			out << (int)value->getBoolValue();
			break;
		}
		case 1:
		{
			out << value->getIntValue();
			break;
		}
		case 2:
		{
			out << value->getFloatValue();
			break;
		}
		case 3:
		{
			out << value->getStringValue();
			break;
		}
		case 4:
			break;
		case 5:
			break;
		default:
		{
			break;
		}
	}
	fprintf(stderr, "Type: \t%s\t\tValue:\t%s\n", (char *)dataName.c_str(), (char *)out.str().c_str());
	
	pthread_mutex_unlock (&sMutex);
}

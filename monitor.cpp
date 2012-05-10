#include <cstdio>

#include "monitor.h"
#include <sstream>
#include <cstdlib>
#include <cstring>

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
    
    argv += optind;
    argc -= optind;
    
    char **subs = NULL;
    if (argc) subs = argv;
    
	//Start game
	GUMonitor *monitor = new GUMonitor(specAddressOfWB, subs, argc);
	
	//Currently waiting for events, loop to keep process from closing
	while(true)
	{
		sleep(5);
	}
	return 0;
}

GUMonitor::GUMonitor(char *whiteboardLocation, char **subscription_list, int n)
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
    if (subscription_list) while (n--)
        wb->subscribeToMessage(*subscription_list++, WB_BIND(GUMonitor::monitorCallback), r);
    else
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
		case WBMsg::TypeBool:
		{
			out << (int)value->getBoolValue();
			break;
		}
		case WBMsg::TypeInt:
		{
			out << value->getIntValue();
			break;
		}
		case WBMsg::TypeFloat:
		{
			out << value->getFloatValue();
			break;
		}
		case WBMsg::TypeString:
		{
			out << value->getStringValue();
			break;
		}
		case WBMsg::TypeArray:
        {
            const vector<int> &vec = value->getArrayValue();
            int n = vec.size();
            out << "( ";
            for (int i = 0; i < n; i++)
            {
                out << vec[i];
                if (i < n-1) out << ", ";
            }
            out << " )";
        }
		default:
		{
			break;
		}
	}
	printf("Type: \t%s\t\tValue:\t%s\n", (char *)dataName.c_str(), (char *)out.str().c_str());
	
	pthread_mutex_unlock (&sMutex);
}

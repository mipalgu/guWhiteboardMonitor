#include <cstdio>

#include "monitor.h"
#include "guwhiteboardgetter.h"
#include <sstream>
#include <cstdlib>
#include <cstring>

using namespace std;
using namespace guWhiteboard;


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
	while(monitor)
	{
		sleep(5);
	}
	return EXIT_FAILURE;
}

GUMonitor::GUMonitor(char *whiteboardLocation, char **subscription_list, int n)
{
#ifdef USE_OLD_WHITEBOARD
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
#else
        watcher = new whiteboard_watcher();

        (void) whiteboardLocation;
        (void) subscription_list;
        (void) n;
#endif
	pthread_mutex_init(&sMutex, NULL);
	//----------------------------------
	
	//Subscriptions
	//----------------------------------
#ifdef USE_OLD_WHITEBOARD
    if (subscription_list) while (n--)
        wb->subscribeToMessage(*subscription_list++, WB_BIND(GUMonitor::monitorCallback), r);
    else
        wb->subscribeToMessage("*", WB_BIND(GUMonitor::monitorCallback), r);
	if(r != Whiteboard::METHOD_OK)
	{
		fprintf(stderr, "Failed to subscribe\n");
	}
#else
        watcher->subscribe(createWBFunctor<GUMonitor>(this, &GUMonitor::callback, kwb_reserved_SubscribeToAllTypes_v));
#endif
}

GUMonitor::~GUMonitor()
{
	
}


void GUMonitor::callback(guWhiteboard::WBTypes t, gu_simple_message *msg)
{
        if (int(t) >= 0 && int(t) < GSW_NUM_TYPES_DEFINED) do
        {
                const char *dataName = WBTypes_stringValues[t];

                if (!dataName)  // no data name, use old wb
                        break;
                try             // try with a new message type
                {
                        pthread_mutex_lock (&sMutex);
                        string value = getmsg(t, msg);
                        printf("Type: \t%s\t\tValue:\t%s\n", dataName, value.c_str());
                        pthread_mutex_unlock (&sMutex);

                        return;
                }
                catch (...)     // no string conversion, fall back to old WB
                {
                        pthread_mutex_unlock (&sMutex);
                }
        } while (0);
        WBMsg wbmsg = Whiteboard::getWBMsg(msg);
        stringstream ss;
        ss << "Old Type " << t;
        monitorCallback(ss.str(), &wbmsg);
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
            size_t n = vec.size();
            out << "( ";
            for (size_t i = 0; i < n; i++)
            {
                out << vec[i];
                if (i < n-1) out << ", ";
            }
            out << " )";
        }
                        break;
		default:
                        break;
	}
	printf("OType:\t%s\t\tValue:\t%s\n", (char *)dataName.c_str(), (char *)out.str().c_str());
	
	pthread_mutex_unlock (&sMutex);
}

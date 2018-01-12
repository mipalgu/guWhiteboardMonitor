#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <cstring>

#include "gu_util.h"
#include "monitor.h"
#include "guwhiteboardgetter.h"

using namespace std;
using namespace guWhiteboard;


int main(int argc, char **argv) 
{
	fprintf(stderr, "\n **** GU WHITEBOARD MONITOR MODULE ****\n (c) 2011 Carl Lusty\n\n");
	
	
	//Get passed in values
	//-----------------------------------
	int op;
	int rwb = -1;

	const char *wbname = NULL;
#ifdef CUSTOM_WB_NAME
	const char *default_name = CUSTOM_WB_NAME;
	wbname = default_name;
#else
	const char *default_name = GSW_DEFAULT_NAME;
#ifndef GSW_IOS_DEVICE
	const char *env = getenv(GSW_DEFAULT_ENV);
	if (env && *env) default_name = env;
#endif
#endif
	while((op = getopt(argc, argv, "r:w:")) != -1)
	{
		switch(op)
		{
			case 'r':
				rwb = atoi(optarg);
				break;
			case 'w':
				wbname = optarg;
				break;
			case '?':			
				fprintf(stderr, "\n\nUsage: guWhiteboardMonitor [OPTION] . . . \n");
				fprintf(stderr, "-r\tID of the remote WB to connect to.\n");
				fprintf(stderr, "-w\tname of the whiteboard to listen on (other than %s).\n", default_name);
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
	GUMonitor *monitor = new GUMonitor(wbname, rwb, subs, argc);
	
	//Currently waiting for events, loop to keep process from closing
	while(monitor)
	{
		sleep(5);
	}
	return EXIT_FAILURE;
}

GUMonitor::GUMonitor(const char *name, int rwb, char **subscription_list, int n)
{
	wbd = NULL;
        if(rwb > 0)
                watcher = new whiteboard_watcher(gswr_new_whiteboard(rwb));
        else if (name)
	{
		wbd = gsw_new_whiteboard(name);
		watcher = new whiteboard_watcher(wbd);
	}
        else watcher = new whiteboard_watcher();

	pthread_mutex_init(&sMutex, NULL);
	//----------------------------------
	
	//Subscriptions
	//----------------------------------
        int i = 0;
        if (subscription_list)
        {
                while (n--)
                {
                        const char *type_name = *subscription_list++;
                        WBTypes type = types_map[type_name];
                        if (!type)
                        {
                                cerr << "Cannot subscribe to unknown type " << type_name << endl;
                                continue;
                        }
                        watcher->subscribe(createWBFunctor<GUMonitor>(this, &GUMonitor::callback, type));
                        ++i;
                }
                if (!i) exit(EXIT_FAILURE);
                DBG(cout << "Subscribed to " << i << " types" << endl);
        }
        else watcher->subscribe(createWBFunctor<GUMonitor>(this, &GUMonitor::callback, kwb_reserved_SubscribeToAllTypes_v));
}

GUMonitor::~GUMonitor()
{
	if (wbd) gsw_free_whiteboard(wbd);
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
                        bool old_wb = false;
                        pthread_mutex_lock (&sMutex);
                        string value = getmsg(t, msg);
                        if (value == "##unsupported##")
                                old_wb = true;
                        else
                                printf("Type:\t%s\t\tValue:\t%s\n", dataName, value.c_str());
                        pthread_mutex_unlock (&sMutex);

                        if (!old_wb) return;
                }
                catch (const char *) // no string conversion, fall back to old WB
                {
                        pthread_mutex_unlock (&sMutex);
                }
                catch (...)     // no string conversion, fall back to old WB
                {
                        pthread_mutex_unlock (&sMutex);
                }
        } while (0);
#ifdef USE_OLD_WHITEBOARD
        WBMsg wbmsg = Whiteboard::getWBMsg(msg);
#endif //USE_OLD_WHITEBOARD 
        gu_simple_whiteboard *wb = get_local_singleton_whiteboard()->wb;
        string name;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-constant-out-of-range-compare"
        if (t < GSW_NUM_TYPES_DEFINED)
        {
                const char *dataName = WBTypes_stringValues[t];
                if (dataName) name = dataName;
                else
                {
                        stringstream ss;
                        ss << "Defined NULL Type " << t;
                        name = ss.str();
                }
        }
        else name = wb->typenames[t].hash.string;

	pthread_mutex_lock (&sMutex);

	std::ostringstream out;
#ifdef USE_OLD_WHITEBOARD
	switch (int(wbmsg.getType()))
	{
		case WBMsg::TypeBool:
		{
			out << int(wbmsg.getBoolValue());
			break;
		}
		case WBMsg::TypeInt:
		{
			out << wbmsg.getIntValue();
			break;
		}
		case WBMsg::TypeFloat:
		{
			out << wbmsg.getFloatValue();
			break;
		}
		case WBMsg::TypeString:
		{
			out << wbmsg.getStringValue();
			break;
		}
		case WBMsg::TypeArray:
                {
                        const vector<int> &vec = wbmsg.getArrayValue();
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
#endif //USE_OLD_WHITEBOARD
	printf("%s %3.3d:\t%s\t\tValue:\t%s\n", t < GSW_NUM_TYPES_DEFINED ? "New" : "Old", t, name.c_str(), out.str().c_str());

	pthread_mutex_unlock (&sMutex);
#pragma clang diagnostic pop
}

#include "EventManager.h"

using namespace std;

EventManager::EventManager()
{
}

bool FunctionComparator::operator()(const EventHandler& a, const EventHandler& b) const
{
  return a.target<void(string,Json::Value)>() < b.target<void(string,Json::Value)>();
}

void EventManager::subscribe(string uri, EventHandler handler)
{
	subscriptions[uri].insert(handler);
}

EventManager& EventManager::getInstance()
{
	static EventManager eventManager;
	return eventManager;
}
	
void EventManager::event(std::string uri, Json::Value payload)
{
	auto ehSet = subscriptions.find(uri);
	
	// has someone subscribed to this event?
	if(ehSet != subscriptions.end())
	{
		for(auto eh : (*ehSet).second)
		{
			eh(uri,payload);
		}
	}
}


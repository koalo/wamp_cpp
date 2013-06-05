#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include <string>
#include <map>
#include <set>
#include <functional>
#include "Json.h"

typedef std::function<void(std::string,Json::Value)> EventHandler;

class FunctionComparator
{
public:
  bool operator()(const EventHandler& a, const EventHandler& b) const;
};

class EventManager
{
public:
	void subscribe(std::string uri, EventHandler handler);
	static EventManager& getInstance();

	void event(std::string uri, Json::Value payload);

	template<typename T>
	void event(std::string uri, T payload)
	{
		event(uri,Json::Value(payload));	
	}

private:
	EventManager();
	std::map<std::string, std::set<EventHandler,FunctionComparator>> subscriptions;
};

#endif


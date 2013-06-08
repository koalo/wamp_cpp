#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_

#include <string>
#include <map>
#include <set>
#include <functional>
#include "Json.h"
#include <queue>
#include <thread>
#include "Topic.h"

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

	void publish(std::string uri, Json::Value payload);

	template<typename T>
	void publish(std::string uri, T payload)
	{
		publish(uri,Json::Value(payload));	
	}

	void pushTopic(AbstractTopic* topic);

private:
	EventManager();
	~EventManager();
	void eventLoop();

	std::map<std::string, std::set<EventHandler,FunctionComparator>> subscriptions;
	std::queue<AbstractTopic*> pendingTopics;
	bool running;
	std::thread eventThread;
	std::mutex lock;
  	std::condition_variable notEmpty;
};

#endif


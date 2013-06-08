#include "Example.h"
#include "EventManager.h"
#include <iostream>

using namespace std;

Example::Example()
: topic("http://example.com/simple/ev")
{
  add<int(Example*,int,int)>("http://example.com/simple/calc#add",&Example::adding);

  eventThread = thread(&Example::eventLoop,this);
}

Example::~Example()
{
}

int Example::adding(int a, int b)
{
  return a+b;
}

void Example::eventLoop()
{
	while(1)
	{
		this_thread::sleep_for(chrono::seconds(5));

		EventManager::getInstance().publish("http://example.com/simple/ev2#test", 55);

		topic.update(12);	
	}
}


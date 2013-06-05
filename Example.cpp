#include "Example.h"
#include "EventManager.h"
#include <iostream>

using namespace std;

Example::Example()
{
  add<int(Example*,int,int)>("http://example.com/simple/calc#add",&Example::adding);

  eventThread = thread(&Example::eventLoop,this);
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
		EventManager::getInstance().event("http://example.com/simple/ev",34);
	}
}

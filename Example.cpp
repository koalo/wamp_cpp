#include "Example.h"
#include <iostream>

using namespace std;

Example::Example()
{
  add<int(Example*,int,int)>("http://example.com/simple/calc#add",&Example::adding);
}

int Example::adding(int a, int b)
{
  return a+b;
}


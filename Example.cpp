#include "Example.h"
#include <iostream>

using namespace std;

Example::Example()
{
  add<void(Example*,int)>("Hallo",&Example::test);
}

int Example::test(int a)
{
  cout << "Guten Tag" << endl;
  return a*3;
}


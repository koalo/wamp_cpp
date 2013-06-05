#include "RPCallable.h"
#include <thread>

class Example : public RPCallable
{
public:
  Example();
  ~Example();
  int adding(int a, int b);

private:
  void eventLoop();
  std::thread eventThread;
};

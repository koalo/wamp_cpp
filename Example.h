#include "RPCallable.h"
#include <thread>
#include "Topic.h"

class Example : public RPCallable
{
public:
  Example();
  ~Example();
  int adding(int a, int b);

private:
  void eventLoop();
  std::thread eventThread;

  Topic<int> topic;
};

#include "Example.h"
#include "Server.h"

int main()
{
  Server server;
  server.start();

  Example g;
  g.run("Hallo");
  g.run("Hallo");
}

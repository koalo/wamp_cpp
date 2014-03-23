#include "DemoServer.h"
#include "WAMPServer.h"

int main()
{
  DemoServer* demo = new DemoServer();
  WAMPServer* server = new WAMPServer();

  server->start();

  std::cout << "Type q to exit" << std::endl;
  while(getchar() != 'q');

  delete demo;

  server->stop();  
  delete server;
}

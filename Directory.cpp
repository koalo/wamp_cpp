#include "Directory.h"

#include <iostream>

using namespace std;

Directory::Directory()
{
}

void Directory::insert(std::string uri, RemoteProcedure rp)
{
	// TODO
  directory[uri] = rp;
}

RemoteProcedure Directory::get(std::string uri)
{
	// TODO
  return directory[uri];
}

Directory& Directory::getInstance()
{
  // instance is static - it lives forever
  static Directory instance;
  return instance;
}


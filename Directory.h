#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include <map>

#include "Json.h"

/**
 * Singleton
 */
class Directory
{
public:
  static Directory& getInstance();

	void insert(std::string uri, RemoteProcedure rp);
	RemoteProcedure get(std::string uri);
	
private:
	Directory();
	std::map<std::string, RemoteProcedure> directory;
};

#endif

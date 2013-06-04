#ifndef JSON_H_
#define JSON_H_

#include <functional>
#include <vector>

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

typedef std::function<Json::Value(std::vector<Json::Value>)> RemoteProcedure;

template<class T>
T convertJson(Json::Value& val)
{
	return T();
}

#endif

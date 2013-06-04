#include "Json.h"

template<>
float convertJson<float>(Json::Value& val)
{
	return val.asFloat();
}

template<>
int convertJson<int>(Json::Value& val)
{
	return val.asInt();
}

template<>
std::string convertJson<std::string>(Json::Value& val)
{
	return val.asString();
}


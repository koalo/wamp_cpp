#include "RPCallable.h"

template<>
float convertJson<float>(Json::Value& val)
{
	return val.asFloat();
}


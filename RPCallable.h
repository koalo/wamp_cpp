#ifndef RPCALLABLE_H_
#define RPCALLABLE_H_

#include <functional>
#include <iostream>
#include <sstream>
#include <map>

#include "Json.h"
#include "Directory.h"

class RPCallable
{
public:
	std::map<std::string,std::function<Json::Value(std::vector<Json::Value>)>> callbacks;

	template<class R, class C, class... Args>
	std::function<Json::Value(std::vector<Json::Value>)> conv(std::function<R(C, Args...)> f)
	{
		return [f,this] (std::vector<Json::Value> vals) 
		{ 
			int i = 0;

			return Json::Value(f((C)this,convertJson<Args>(vals[i++])...));
		};
	}

	template<class C, class... Args>
	std::function<Json::Value(std::vector<Json::Value>)> conv(std::function<void(C, Args...)> f)
	{
		return [f,this] (std::vector<Json::Value> vals) 
		{ 
			int i = 0;
			f((C)this,convertJson<Args>(vals[i++])...);
			return Json::Value(12);
		};
	}

	template<class G, class T>
	void add(std::string uri, T&& t)
	{
		Directory::getInstance().insert(uri, conv(static_cast<std::function<G>>(t)));
	}

	/*
	void run(std::string uri)
	{
		std::vector<Json::Value> v;
		callbacks[uri](v);
	}
	*/
};

#endif

#ifndef RPCALLABLE_H_
#define RPCALLABLE_H_

#include <functional>
#include <iostream>
#include <sstream>
#include <map>
#include <type_traits>

#include "Json.h"
#include "Directory.h"

template<int... Is>
struct seq { };

template<int N, int... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

template<int... Is>
struct gen_seq<0, Is...> : seq<Is...> { };

template< class C, typename R, class... Args, int... Is>
//void callByVector(C* self, std::function<void(Args...)> f,
R callByVector(C* self, R (C::*f)(), std::vector<Json::Value>& arguments, seq<Is...>)
{
	return (self->*f)(convertJson<Args...>(arguments[Is])...);
	//f(self,convertJson<Args...>(arguments[Is])...);
}

template< class C, typename R, class... Args>
//void callByVector(C* self, std::function<void(Args...)> f, std::vector<Json::Value>& arguments)
R callByVector(C* self, R (C::*f)(), std::vector<Json::Value>& arguments)
{
	return callByVector(self, f, arguments, gen_seq<sizeof...(Args)>());
}



template< class C, typename R >
std::function<Json::Value(std::vector<Json::Value>)> getJsonLambda(C* self, R (C::*f)(),
		typename std::enable_if<!std::is_void<R>::value >::type* = 0)
{
	return [f,self] (std::vector<Json::Value> vals) -> Json::Value
	{ 
		return Json::Value((self->*f)());
	};
}

template< class C, typename R >
std::function<Json::Value(std::vector<Json::Value>)> getJsonLambda(C* self, R (C::*f)(), 
		typename std::enable_if<std::is_void<R>::value >::type* = 0)
{
	return [f,self] (std::vector<Json::Value> vals) -> Json::Value
	{ 
		(self->*f)();
		return Json::Value();
	};
}

template< class C, typename R, typename... Args >
std::function<Json::Value(std::vector<Json::Value>)> getJsonLambda(C* self, R (C::*f)(Args...),
		typename std::enable_if<!std::is_void<R>::value >::type* = 0)
{
	return [f,self] (std::vector<Json::Value> vals) -> Json::Value
	{ 
		return Json::Value(callByVector(self,f,vals));
	};
}

template< class C, typename R, typename... Args >
std::function<Json::Value(std::vector<Json::Value>)> getJsonLambda(C* self, R (C::*f)(Args...), 
		typename std::enable_if<std::is_void<R>::value >::type* = 0)
{
	return [f,self] (std::vector<Json::Value> vals) -> Json::Value
	{ 
		callByVector(self,f,vals);
		return Json::Value();
	};
}

template<class C>
class RPCallable
{
private:
/*
	template<class... Args, int... Is>
	void callByVector(std::function<void(Args...)> f,
		C self, std::vector<Json::Value>& arguments, seq<Is...>)
	{
  		f(self,convertJson<Args...>(arguments[Is])...);
	}

	template<class... Args>
	void callByVector(std::function<void(Args...)> f, C self, std::vector<Json::Value>& arguments)
	{
  		callByVector(f, arguments, gen_seq<sizeof...(Args)>());
	}

	template<class... Args>
	std::function<Json::Value(std::vector<Json::Value>)> conv(std::function<void(Args...)> f)
	{
		return [f,this] (std::vector<Json::Value> vals) 
		{ 
			callByVector(f,this,vals);
			return Json::Value();
		};
	}
*/

protected:

template< typename R >
void addRemoteProcedure(std::string uri, R (C::*f)()) {
/*
		auto fun = [f,this] (std::vector<Json::Value> vals) 
		{ 
			C* self = static_cast<C*>(this);
			if(std::is_void<R>)
			{
				(self->*f)();
				return Json::Value();
			}
			else
			{
				return Json::Value((self->*f)());
			}
		};
*/

		C* self = static_cast<C*>(this);
		Directory::getInstance().insert(uri, getJsonLambda(self,f));
}

template< typename R, typename... Args >
void addRemoteProcedure(std::string uri, R (C::*f)(Args...)) {
/*
		auto fun = [f,this] (std::vector<Json::Value> vals) 
		{ 
			return callByVector(f);
		};

		Directory::getInstance().insert(uri, fun);
*/
}


	/*template<class G, class T>
	void addRemoteProcedure(std::string uri, T&& t)
	{
		auto fun = static_cast<std::function<G>>(t);
		Directory::getInstance().insert(uri, conv(fun));
	}
*/
/*
	template<typename R, typename... Args>
	void addRemoteProcedure(std::string uri, std::function<R(C*,Args...)>& fun)
	{
		//auto fun = static_cast<std::function<R(Args...)>>(t);
		Directory::getInstance().insert(uri, conv(fun));
	}
*/

	void addConnectionHandler(std::function<void(std::string)> handler)
	{
		Directory::getInstance().addConnectionHandler(handler);
	}

private:
	std::map<std::string,std::function<Json::Value(std::vector<Json::Value>)>> callbacks;

#if 0
	template<class R, class C, class... Args>
	std::function<Json::Value(std::vector<Json::Value>)> conv(std::function<R(C, Args...)> f)
	{
		return [f,this] (std::vector<Json::Value> vals) 
		{ 
			int i = 0;
			//return Json::Value(f((C)this,convertJson<Args>(vals[i++])...));
			return Json::Value(f((C)this,convertJson<Args>(vals[i++])...));
			//return Json::Value(f((C)this,popArg<Args>(vals)...));
			//return Json::Value(f((C)this,popArg<Args>(vals)...));
		};
	}

	template<class C, class... Args>
	std::function<Json::Value(std::vector<Json::Value>)> conv(std::function<void(C, Args...)> f)
	{
		return [f,this] (std::vector<Json::Value> vals) 
		{ 
/*
if(vals.size() == 3)
{
std::cout << vals[0] << " x " << vals[1] << " x " << vals[2] << std::endl;
}
*/
			int i = 0;
			//f((C)this,convertJson<Args>(vals[i++])...);
			f((C)this,convertJson<Args>(vals[i++])...);
//			f((C)this,popArg<Args>(vals)...);
			f((C)this,popArg<Args>(vals)...);
			return Json::Value();
		};
	}
#endif

/*
	template<class R, class C, class... Args>
	std::function<Json::Value(std::vector<Json::Value>)> conv(std::function<R(C, Args...)> f)
	{
		return [f,this] (std::vector<Json::Value> vals) 
		{ 
			auto b = bind(f,(C)this);
			//while(!vals.empty())
			//{
				b = gg<R,Args...>(static_cast<std::function<R(Args...)>>(b),vals);
			//}
			return Json::Value(b());
		};
	}
*/
};

#endif

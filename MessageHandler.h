#ifndef MESSAGEHANDLER_H_
#define MESSAGEHANDLER_H_
#include <iostream>
#include <sstream>
#include <functional>

#include "Json.h"

class MessageHandler
{
public:
  enum WAMPMessage
  {
	WELCOME,
	PREFIX,
	CALL,
	CALLRESULT,
	CALLERROR,
	SUBSCRIBE,
	UNSUBSCRIBE,
	PUBLISH,
	EVENT
  };

  void sendCallResult(std::string callID, Json::Value result);
  void handleCall(std::string callID, std::string uri, std::vector<Json::Value> values);
  void sendEvent(std::string uri, Json::Value payload);
  void subscribe(std::string uri);
  void receiveMessage(std::string msg);
  void registerSend(std::function<void(std::string)> send);

  std::function<void(std::string)> sendFun;
};
#endif

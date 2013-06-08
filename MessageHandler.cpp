#include "MessageHandler.h"
#include "Directory.h"
#include "EventManager.h"

using namespace std;
using namespace std::placeholders;

void MessageHandler::sendCallResult(string callID, Json::Value result)
{
	Json::FastWriter writer;
	stringstream output;
	output << "[" << CALLRESULT << ",\"" << callID << "\"," << writer.write(result) << "]";

	if(sendFun)
	{
	  sendFun(output.str());
	}
}



void MessageHandler::handleCall(string callID, string uri, vector<Json::Value> values)
{
	RemoteProcedure rp = Directory::getInstance().get(uri);
	Json::Value result;

	if(rp)
	{
		// TODO what if values does not match??
		// TODO future!!
		result = rp(values);
		sendCallResult(callID,result);
	}
	else
	{
		// send error!
	}
}

void MessageHandler::sendEvent(string uri, Json::Value payload)
{
	Json::FastWriter writer;
	stringstream output;
	output << "[" << EVENT << ",\"" << uri << "\"," << writer.write(payload) << "]";
	cout << output << endl;

	if(sendFun)
	{
	  sendFun(output.str());
	}
}

void MessageHandler::subscribe(string uri)
{
  auto eventHandler = bind(&MessageHandler::sendEvent,this,_1,_2); 
  //EventManager::getInstance().subscribe(uri,static_cast<EventHandler>(eventHandler));
  EventManager::getInstance().subscribe(uri,eventHandler);
}

void MessageHandler::receiveMessage(std::string msg)
{
	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	stringstream input;
	input << msg;
	bool parsingSuccessful = reader.parse( msg, root );
	if ( !parsingSuccessful )
	{
		std::cout  << "Failed to parse configuration\n"
			<< reader.getFormattedErrorMessages();
		return;
	}

	if(root.size() < 1 || !root[0].isInt())
	{
		cerr << "Wrong message format" << endl;
	}
	
	vector<Json::Value> values;

	switch(root[0].asInt())
	{
	case CALL:
		if(root.size() < 3)
		{
		  cerr << "Too few elements in message for CALL" << endl;
		}

		for(int i = 3; i < root.size(); i++)
		{
			values.push_back(root[i]);
		}

		handleCall(root[1].asString(),root[2].asString(),values);
		break;
	case SUBSCRIBE:
		if(root.size() < 2)
		{
		  cerr << "Too few elements in message for SUBSCRIBE" << endl;
		}
		
		subscribe(root[1].asString());

	default:
		cerr << "Not implemented" << endl;
	}
}

void MessageHandler::registerSend(function<void(std::string)> send)
{
  sendFun = send; 
}


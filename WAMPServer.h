#ifndef WAMPSERVER_H_
#define WAMPSERVER_H_
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <boost/bimap.hpp>

#include "MessageHandler.h"

#include <set>
#include <functional>
#include <thread>
#include <mutex>

#include <iostream>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using websocketpp::connection_hdl;

class WAMPServer {
public:
	typedef websocketpp::server<websocketpp::config::asio> server;

	// pull out the type of messages sent by our config
	typedef server::message_ptr message_ptr;

	//typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;
	typedef std::set<connection_hdl> con_list;

	// Define a callback to handle incoming messages
	void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
	bool validate(connection_hdl hdl);
	void on_open(connection_hdl hdl);
	void on_close(connection_hdl hdl);
	void send(std::string client, std::string msg);
	void start();
	void stop();

private:
	void thread();
	static std::string generateRandomString();
	std::thread serverThread;

	server wserver;
	con_list connections;
	MessageHandler handler;
	boost::bimap<std::string, connection_hdl> clients;
};

#endif

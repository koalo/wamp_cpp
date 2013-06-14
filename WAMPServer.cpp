#include "MessageHandler.h"

#include <iostream>
#include <sstream>
#include <random>

#include "WAMPServer.h"

#include "Directory.h"

using namespace std;

void WAMPServer::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
	handler.receiveMessage(clients.right.at(hdl),msg->get_payload());

	try {
		wserver.send(hdl, msg->get_payload(), msg->get_opcode());
	} catch (const websocketpp::lib::error_code& e) {
		std::cout << " failed because: " << e  
			<< "(" << e.message() << ")" << std::endl;
	}
}

bool WAMPServer::validate(connection_hdl hdl) {
	server::connection_ptr con = wserver.get_con_from_hdl(hdl);

	std::cout << "Cache-Control: " << con->get_request_header("Cache-Control") << std::endl;

	const std::vector<std::string> & subp_requests = con->get_requested_subprotocols();
	std::vector<std::string>::const_iterator it;

	for (it = subp_requests.begin(); it != subp_requests.end(); ++it) {
		std::cout << "Requested: " << *it << std::endl;
	}

	if (subp_requests.size() > 0) {
		con->select_subprotocol(subp_requests[0]);
	}

	return true;
}

string WAMPServer::generateRandomString()
{
	stringstream ss;
	string chars(
			"abcdefghijklmnopqrstuvwxyz"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"1234567890");
	random_device rng;
	uniform_int_distribution<> index_dist(0, chars.size() - 1);
	for(int i = 0; i < 16; ++i) {
		ss << chars[index_dist(rng)];
	}
	return ss.str();
}

void WAMPServer::on_open(connection_hdl hdl) {

	connections.insert(hdl);
	try {
		string sessionId = generateRandomString();
		stringstream ss;
		clients.insert(boost::bimap< std::string, connection_hdl >::value_type(sessionId,hdl));
		ss << "[0, \"" << sessionId << "\", 1, \"wamp_cpp/0.0.0.1\"]";
		wserver.send(hdl, ss.str(), websocketpp::frame::opcode::text);
		Directory::getInstance().connectionEstablished(sessionId);
	} catch (const websocketpp::lib::error_code& e) {
		std::cout << "Open connection failed because: " << e  
			<< " (" << e.message() << ")" << std::endl;
	}
}

void WAMPServer::on_close(connection_hdl hdl) {
	connections.erase(hdl);
}

void WAMPServer::send(std::string client, std::string msg)
{
	try {
		auto hdl = clients.left.at(client);
		if(!hdl.lock().get())
		{
			cout << "Session to " << client << " finished" << endl;
			clients.left.erase(client);
			return;
		}

		wserver.send(hdl, msg, websocketpp::frame::opcode::text);
	} catch (const websocketpp::lib::error_code& e) {
		std::cout << "Send failed because: " << e  
			<< " (" << e.message() << ")" << std::endl;
	} catch(std::out_of_range e) {
		// does not matter -> don't send anything
	}
}

void WAMPServer::start()
{
	serverThread = std::thread(&WAMPServer::thread,this);
}

void WAMPServer::stop()
{
	wserver.stop();
	serverThread.join();
}

void WAMPServer::thread()
{
	handler.registerSend(bind(&WAMPServer::send,this,::_1,::_2));

	try {
		// Set logging settings
		//wserver.set_access_channels(websocketpp::log::alevel::all);
		wserver.clear_access_channels(websocketpp::log::alevel::all);

		// Initialize ASIO
		wserver.init_asio();

		// Register our message handler
		wserver.set_message_handler(bind(&WAMPServer::on_message,this,::_1,::_2));
		wserver.set_validate_handler(bind(&WAMPServer::validate,this,::_1));
		wserver.set_open_handler(bind(&WAMPServer::on_open,this,::_1));
		wserver.set_close_handler(bind(&WAMPServer::on_close,this,::_1));

		// Listen on port 9002
		wserver.listen(9002);

		// Start the server accept loop
		wserver.start_accept();

		// Start the ASIO io_service run loop
		wserver.run();
	} catch (const std::exception & e) {
		std::cout << e.what() << std::endl;
	} catch (websocketpp::lib::error_code e) {
		std::cout << e.message() << std::endl;
	} catch (...) {
		std::cout << "other exception" << std::endl;
	}
}


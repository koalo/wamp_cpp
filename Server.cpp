#include "MessageHandler.h"

#include <iostream>

#include "Server.h"


	void Server::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
		std::cout << "on_message called with hdl: " << hdl.lock().get() 
			<< " and message: " << msg->get_payload()
			<< std::endl;

		handler.receiveMessage(msg->get_payload());

		try {
			wserver.send(hdl, msg->get_payload(), msg->get_opcode());
		} catch (const websocketpp::lib::error_code& e) {
			std::cout << "Echo failed because: " << e  
				<< "(" << e.message() << ")" << std::endl;
		}
	}

	bool Server::validate(connection_hdl hdl) {
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

	void Server::on_open(connection_hdl hdl) {

		connections.insert(hdl);
		try {
			wserver.send(hdl, "[0, \"abcdef\", 1, \"wamp_cpp/0.0.0.1\"]", websocketpp::frame::opcode::text);
		} catch (const websocketpp::lib::error_code& e) {
			std::cout << "Echo failed because: " << e  
				<< "(" << e.message() << ")" << std::endl;
		}
	}

	void Server::on_close(connection_hdl hdl) {
		connections.erase(hdl);
	}

	void Server::send(std::string msg)
	{
		try {
			// TODO connections?
			for(auto it : connections)
			{
				wserver.send(it, msg, websocketpp::frame::opcode::text);
			}
		} catch (const websocketpp::lib::error_code& e) {
			std::cout << "Echo failed because: " << e  
				<< "(" << e.message() << ")" << std::endl;
		}
	}

	void Server::start()
	{
		handler.registerSend(bind(&Server::send,this,::_1));

		try {
			// Set logging settings
			wserver.set_access_channels(websocketpp::log::alevel::all);
			wserver.clear_access_channels(websocketpp::log::alevel::frame_payload);

			// Initialize ASIO
			wserver.init_asio();

			// Register our message handler
			wserver.set_message_handler(bind(&Server::on_message,this,::_1,::_2));
			wserver.set_validate_handler(bind(&Server::validate,this,::_1));
			wserver.set_open_handler(bind(&Server::on_open,this,::_1));
			wserver.set_close_handler(bind(&Server::on_close,this,::_1));

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


#pragma once

#include <algorithm>
#include <deque>

#include <boost/asio.hpp>

#include "DataBase.h"
#include "CmdHandler.h"

constexpr auto MAX_DATA_SIZE = 1024;

namespace ba = boost::asio;

class RequestHandler : public std::enable_shared_from_this<RequestHandler> 
{
private:
	ba::ip::tcp::socket Socket;
	std::array<char, MAX_DATA_SIZE> Data = {0};
	DataBase& DB;
	CommandParser CmdParser;

	std::deque<std::string> Resps;

	void do_read();
	void do_write();


public:
	explicit RequestHandler(ba::ip::tcp::socket _Socket, DataBase& _DB)
		: Socket(std::move(_Socket)), DB(_DB)
	{
		//memset(&Data, 0, sizeof(Data));
		//std::fill(Data.begin(), Data.end(), 0);
	}

	void Start();
};


class JoinServer 
{
private:
	void do_accept();

	boost::asio::ip::tcp::acceptor Acceptor;
	ba::ip::tcp::socket Socket;
	DataBase& DB;

public:
	JoinServer(ba::io_service& _io_service, const ba::ip::tcp::endpoint& _endpoint,
		DataBase& _DB)
		: Acceptor(_io_service, _endpoint), Socket(_io_service), DB(_DB)
	{
		do_accept();
	}

};
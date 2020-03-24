#include <string>
#include <iostream>
#include <sstream>

#include "common.h"
#include "server.h"

using namespace std;

void RequestHandler::Start()
{
	do_read();
}

bool HandleInsertCommand(Command &cmd, DataBase& DB, int &nd)
{
	char table[16], name[64];
	sscanf(cmd.Info.c_str(), "%s%i%s", table, &nd, name);
	table[15] = name[63] = '\0';
	return DB.Insert(table, nd, name);
}

void RequestHandler::do_read() 
{
	//auto self(shared_from_this());
	auto self(shared_from_this());
	Socket.async_read_some(
		ba::buffer(Data),
		[this, self](boost::system::error_code ec, size_t length)
		{
			if (!ec) 
			{
				if (length)
				{
					MY_DEBUG_ONLY(cout << "Data = ";
					char b[MAX_DATA_SIZE + 1] = { '\0' };
					memcpy(b, &Data, length);
					cout << b << endl;)

						auto cmds = CmdParser.ParseAll(Data.data(), length);

						for (auto &cmd : cmds)
						{
							bool write_in_progress = !Resps.empty();

							switch (cmd.Type)
							{
							case CommandEnum::UNKNOWN:
								Resps.emplace_back("Err unknown command\n");
								break;

							case CommandEnum::INSERT:
								int nd;
								if ( HandleInsertCommand(cmd, DB, nd) )
									Resps.emplace_back("OK\n");
								else
									Resps.emplace_back("Err insertation with id " + to_string(nd) + "\n");
								break;

							case CommandEnum::TRUNCATE:
								if ( DB.Truncate(string(cmd.Info.c_str())) )
									Resps.emplace_back("OK\n");
								else
									Resps.emplace_back("Err Truncate\n");
								break;

							case CommandEnum::INTERSECTION:
								{
								auto r = DB.Intersection();
								stringstream ss;
								ss << r;
								Resps.emplace_back(ss.str() + "OK\n");
								}
								break;

							case CommandEnum::SYMMETRIC_DIFFERENCE:
								{
								auto r = DB.SymmetricDifference();
								stringstream ss;
								ss << r;
								Resps.emplace_back(ss.str() + "OK\n");
								}
								break;
							}

							if (!write_in_progress)
							{
								do_write();
							}
						}

				}

				do_read();
			}
		}
	);
}

void RequestHandler::do_write() 
{
	auto self(shared_from_this());
	boost::asio::async_write(Socket,
		boost::asio::buffer(Resps.front().data(), Resps.front().length()),
		[this, self](boost::system::error_code ec, size_t /*length*/)
		{
			if (!ec) 
			{
				Resps.pop_front();
				if (!Resps.empty())
				{
					do_write();
				}
			}
		}
	);
}

void JoinServer::do_accept() 
{
	Acceptor.async_accept(Socket,
		[this](boost::system::error_code ec)
		{
			if (!ec) 
			{
				std::make_shared<RequestHandler>(std::move(Socket), DB)->Start();
			}

			do_accept();
		}
	);
}
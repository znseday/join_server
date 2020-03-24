#include <iostream>
#include <vector>
//#include <cassert>
#include <fstream>

#include "common.h"
#include "server.h"
#include "CmdHandler.h"
#include "DataBase.h"

using namespace std;

namespace ba = boost::asio;

bool IsDebugOutput = false;

void TestDataBaseCommands();
void TestCmdParser();

int main(int argc, char *argv[])
{
	MY_DEBUG_ONLY(cout << "Homework join_server (DEBUG detected)" << endl;)

	#if (defined WIN32) || (defined WIN64)
		cout << MY_P_FUNC << endl;              // for debugging
	#else
		//istream &i_stream = cin;
	#endif

	try
	{
		if (argc < 2)
		{
			cerr << "There must be at least 1 parameter: join_server <port>" << endl;
			exit(0);
		}
		int Port = -1;
		sscanf(argv[1], "%i", &Port);

		if (Port < 1)
		{
			cerr << "My error: something's wrong with Port number" << endl;
			exit(0);
		}

		for (int i = 1; i < argc; i++)
			if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "d") == 0)
				IsDebugOutput = true;

		MY_DEBUG_ONLY(TestDataBaseCommands();)           // for debugging
		MY_DEBUG_ONLY(TestCmdParser();)           // for debugging
		
		
		ba::io_service io_context;

		ba::ip::tcp::endpoint endpoint(ba::ip::tcp::v4(), Port);
		DataBase DB;

		JoinServer Server(io_context, endpoint, DB);

		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	//cout << "Done" << endl;

    return 0;
}
//--------------------------------------------------------------------

void TestDataBaseCommands()
{
	cout << endl << "TestDataBaseCommands:" << endl << endl;

	DataBase DB;

	DB.Insert("A", 0, "lean");
	DB.Insert("A", 1, "sweater");
	DB.Insert("A", 2, "frank");
	DB.Insert("A", 3, "violation");
	DB.Insert("A", 4, "quality");
	DB.Insert("A", 5, "precision");
	DB.Insert("B", 3, "proposal");
	DB.Insert("B", 4, "example");
	DB.Insert("B", 5, "lake");
	DB.Insert("B", 6, "flour");
	DB.Insert("B", 7, "wonder");
	DB.Insert("B", 8, "selection");

	auto res = DB.Intersection();
	cout << res << endl;

	res = DB.SymmetricDifference();
	cout << res << endl;

	cout << "Done." << endl;
}

void TestCmdParser()
{
	cout << endl << "TestCmdHandler:" << endl << endl;

	CommandParser parser;

	string s = "1";
	cout << "L = " << s.size() << endl;

	string TestString = "INSERT A 0 lean\nINSERT B 6 flour\nINS";

	TestString = "INSERT A 0 lean\nINSERT A 0 understand\nINSERT A 1 sweater\nINSERT A 2 frank\nINSERT B 6 flour\nINSERT B 7 wonder\nINSERT B 8 selection\n";

	auto cmds = parser.ParseAll(TestString.data(), TestString.size());

	cout << "Done." << endl;
}

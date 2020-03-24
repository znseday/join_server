#include <string_view>
#include <algorithm>

#include "CmdHandler.h"

using namespace std;

ostream& operator<<(ostream& s, const Command &ob)
{
	s << (int)ob.Type << " " << ob.Info;
	return s;
}

Command ParseLine(string line)
{
	Command res;
	if (auto pos = line.find_first_of(' '); pos != string::npos)
	{
		res.Info = line.substr(pos+1, line.size()-pos-1);
		line = line.substr(0, pos);
	}
	
	if (line == "INSERT")
		res.Type = CommandEnum::INSERT;
	else if (line == "TRUNCATE")
		res.Type = CommandEnum::TRUNCATE;
	else if (line == "INTERSECTION")
		res.Type = CommandEnum::INTERSECTION;
	else if (line == "SYMMETRIC_DIFFERENCE")
		res.Type = CommandEnum::SYMMETRIC_DIFFERENCE;
	else
		res.Type = CommandEnum::UNKNOWN;

	return res;
}


std::vector<Command> CommandParser::ParseAll(char *data, size_t data_size)
{
	string_view sv(data, data_size);
	std::vector<Command> res;
	while ( !sv.empty() ) 
	{
		auto pos = sv.find('\n');
		if ( !Buffer.empty() ) 
		{
			Buffer += sv.substr(0, pos);
			if (pos != string_view::npos) 
			{
				res.push_back(ParseLine(Buffer));
			}
			Buffer.clear();
		}
		if (pos != string_view::npos) 
		{
			string_view svt(sv.substr(0, pos));
			string t(svt);
			res.push_back(ParseLine(t));
		}
		else 
		{
			Buffer += sv.substr(0, pos);
			break;
		}
		sv.remove_prefix(min(pos + 1, sv.size()));
	}
	return res;
}
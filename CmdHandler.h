#pragma once

#include <string>
#include <vector>
#include <iostream>


enum class CommandEnum
{
	UNKNOWN,
	INSERT,
	TRUNCATE,
	INTERSECTION,
	SYMMETRIC_DIFFERENCE
};

class Command
{
public:
	CommandEnum Type;
	std::string Info;
};
std::ostream& operator<<(std::ostream& s, const Command& ob);

Command ParseLine(std::string line);

class CommandParser 
{
private:
	std::string Buffer;

public:

	std::vector<Command> ParseAll(char* data, std::size_t data_size);
};

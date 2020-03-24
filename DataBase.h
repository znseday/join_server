#pragma once

#include <string>
#include <map>
#include <iostream>
#include <array>


struct TwoFields 
{
	std::string FieldA;
	std::string FieldB;
	TwoFields(std::string _FieldA, std::string _FieldB) :
		FieldA(std::move(_FieldA)), FieldB(std::move(_FieldB))
	{}
};
bool operator==(const TwoFields &x, const TwoFields &y);
std::ostream& operator<<(std::ostream &s, const TwoFields &ob);

using ResponseTable = std::map<int, TwoFields>;
std::ostream& operator<<(std::ostream& s, const ResponseTable& ob);


class Table 
{
private:
	std::map<int, std::string> Data;

public:

	bool Insert(int id, std::string name);
	void Truncate();

	const std::map<int, std::string>& GetData() const { return Data;}
};

class DataBase 
{
private:
	Table TableA;
	Table TableB;

public:

	bool Insert(const std::string &tableName, int id, std::string name);
	bool Truncate(const std::string &tableName);

	ResponseTable Intersection() const;
	ResponseTable SymmetricDifference() const;
};



#include "DataBase.h"

#include <algorithm>

using namespace std;

bool operator==(const TwoFields &x, const TwoFields &y)
{
	return (x.FieldA == y.FieldA) && (x.FieldB == y.FieldB);
}

ostream& operator<<(ostream& s, const TwoFields &ob) 
{
	s << ob.FieldA << ',' << ob.FieldA;
	return s;
}

ostream& operator<<(ostream& s, const ResponseTable& ob)
{
	for (const auto &v : ob)
		cout << v.first << "," << v.second.FieldA << "," << v.second.FieldB << endl;

	return s;
}


bool Table::Insert(int id, string name) 
{
	auto [it, res] = Data.try_emplace(id, move(name));
	return res;
}

void Table::Truncate() 
{
	Data.clear();
}


bool DataBase::Insert(const std::string &tableName, int id, std::string name)
{
	if (tableName == "A")
		return TableA.Insert(id, move(name));
	else if (tableName == "B")
		return TableB.Insert(id, move(name));
	else
		return false;
}

bool DataBase::Truncate(const std::string &tableName) 
{
	if (tableName == "A")
	{
		TableA.Truncate();
		return true;
	}
	else if (tableName == "B")
	{
		TableB.Truncate();
		return true;
	}
	else
		return false;
}

ResponseTable DataBase::Intersection() const 
{
	//  Complexity: O( A.size() + B.size() )

	ResponseTable res;
	auto A_first = TableA.GetData().begin();
	auto A_last = TableA.GetData().end();
	auto B_first = TableB.GetData().begin();
	auto B_last = TableB.GetData().end();

	while (A_first != A_last && B_first != B_last)
	{
		if (A_first->first < B_first->first)
			++A_first;
		else if (A_first->first > B_first->first)
			++B_first;
		else  // (A_first->first == B_first->first)
		{
			res.try_emplace(res.end(), A_first->first, A_first->second, B_first->second);
			++A_first;
			++B_first;
		}
	}
	return res;
}

ResponseTable DataBase::SymmetricDifference() const
{
	//  Complexity: O( A.size() + B.size() )

	ResponseTable res;
	auto A_first = TableA.GetData().begin();
	auto A_last  = TableA.GetData().end();
	auto B_first = TableB.GetData().begin();
	auto B_last  = TableB.GetData().end();

	while (A_first != A_last && B_first != B_last)
	{
		if (A_first->first < B_first->first)
		{
			res.try_emplace(res.end(), A_first->first, A_first->second, string());
			++A_first;
		}
		else if (A_first->first > B_first->first)
		{
			res.try_emplace(res.end(), B_first->first, string(), B_first->second);
			++B_first;
		}
		else // (A_first->first == B_first->first) 
		{
			++A_first;
			++B_first;
		}
	}

	for (; B_first != B_last; ++B_first)
	{
		res.try_emplace(res.end(), B_first->first, string(), B_first->second);
	}
	return res;
}
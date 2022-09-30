#pragma once
using namespace std;
#include <string>

// just wanted a more explicit way of getting and setting the data of each card type
struct CardData
{
	CardData(string n, string p, int l)
	{
		name = n;
		prefix = p;
		length = l;
	}

	string name;
	string prefix;
	int length;
};
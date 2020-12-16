#pragma once

#include <map>
#include <fstream>

#include "tstring.h"

class TIniFile
{
private:
	std::tstring filename;
	std::map<std::tstring, std::tstring> dic;
public:
	TIniFile(std::tstring filename)throw (std::runtime_error);

	std::tstring GetValue(std::tstring key);

	void SetValue(std::tstring key, std::tstring value);

	void SetValue(std::tstring key, int value);

	void SaveToFile()throw (std::runtime_error);
};


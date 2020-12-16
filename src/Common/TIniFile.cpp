#include "TIniFile.h"

#include <stdexcept>

using namespace std;

TIniFile::TIniFile(std::tstring filename) throw (runtime_error) :filename(filename)
{
	tifstream ifs(filename);

	locale::global(locale(""));
	ifs.imbue(locale(""));

	if (ifs.fail())//ûȨ�޻򲻴���
	{
		//�����հ�ini�ļ�
		ofstream ofs(filename);
		if (ofs.fail())
			throw runtime_error("Can't not write " + to_string(filename));
		ofs.close();
	}
	else
	{
		//����
		tstring line;
		while (getline(ifs, line))
		{
			auto equal_pos = line.find('=');
			if (equal_pos != tstring::npos)
			{
				tstring key = line.substr(0, equal_pos);
				tstring value = line.substr(equal_pos + 1);

				dic[key] = value;
			}
		}
	}
}

std::tstring TIniFile::GetValue(std::tstring key)
{
	return dic[key];
}

void TIniFile::SetValue(std::tstring key, std::tstring value)
{
	dic[key] = value;
}

void TIniFile::SetValue(std::tstring key, int value)
{
	dic[key] = tto_string(value);
}

void TIniFile::SaveToFile()throw (std::runtime_error)
{
	tofstream ofs(filename);

	locale::global(locale(""));
	ofs.imbue(locale(""));

	if (ofs.fail())
		throw runtime_error("д��" + to_string(filename) + "ʧ�ܡ�");

	for (auto& pr : dic)
	{
		ofs << pr.first << TEXT("=") << pr.second << endl;
	}

	ofs.close();
}

#include "MyBase64.h"
#include <Windows.h>

namespace MyBase64
{
	using namespace std;

	const char dic[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

	inline unsigned char Lookup(unsigned char c)
	{
		if (c >= 'A'&& c <= 'Z')
			return c - 'A';
		if (c >= 'a'&& c <= 'z')
			return c - 'a' + 26;
		if (c >= '0'&& c <= '9')
			return c - '0' + 52;
		switch (c)
		{
		case '+':return 62;
		case '/':return 63;
		}
		throw std::runtime_error("DecodeBase64 error.");
	}

	void EncodeToBase64(std::ostream& out, const std::stringstream& in, int max_line_count)
	{
		out << EncodeToBase64(in.str(),max_line_count);
	}

	void EncodeToBase64(std::ostream& out, const std::string& in, int max_line_count)
	{
		out << EncodeToBase64(in,max_line_count);
	}

	std::string EncodeToBase64(const char* data, size_t len,int max_line_count)
	{
		int prNum = len / 3;
		std::string ret;
		int line_count = 0;
		for (int i = 0; i < prNum; ++i)
		{
			if (line_count + 4 >= max_line_count)
			{
				ret += "\r\n";
				line_count = 0;
			}

			ret += dic[(unsigned char)data[i * 3] >> 2];
			ret += dic[(((unsigned char)data[i * 3] & 0x3) << 4) | ((unsigned char)data[i * 3 + 1] >> 4)];
			ret += dic[(((unsigned char)data[i * 3 + 1] & 0xF) << 2) | ((unsigned char)data[i * 3 + 2] >> 6)];
			ret += dic[(unsigned char)data[i * 3 + 2] & 0x3F];
			line_count += 4;
		}

		if (len % 3 == 1)
		{
			if (line_count + 4 >= max_line_count)
			{
				ret += "\r\n";
				line_count = 0;
			}

			unsigned char c1 = data[prNum * 3];
			unsigned char c2 = data[prNum * 3 + 1];
			ret += dic[c1 >> 2];
			ret += dic[((c1 & 0x3) << 4) | (0 >> 4)];
			ret += "==";
			return ret;
		}

		if (len % 3 == 2)
		{
			if (line_count + 4 >= max_line_count)
			{
				ret += "\r\n";
				line_count = 0;
			}

			unsigned char c1 = data[prNum * 3];
			unsigned char c2 = data[prNum * 3 + 1];
			ret += dic[c1 >> 2];
			ret += dic[((c1 & 0x3) << 4) | (c2 >> 4)];
			ret += dic[((c2 & 0xF) << 2) | (0 >> 6)];
			ret += '=';
			return ret;
		}
		return ret;
	}

	std::string EncodeToBase64(std::string in, int max_line_count)
	{
		return EncodeToBase64(in.c_str(), in.size(), max_line_count);
	}

	std::string DecodeBase64(const std::string &input)
	{
		std::string ret;
		int state = 0;
		unsigned char c1 = 0;
		unsigned char c2 = 0;
		unsigned char c3 = 0;
		unsigned char c4 = 0;
		for (unsigned char c : input)
		{
			switch (state)
			{
			case 0:
				if (c == '\r' || c == '\n' || c == ' ')
					break;
				c1 = Lookup(c);
				state = 1;
				break;
			case 1:
				if (c == '\r' || c == '\n' || c == ' ')
					break;
				c2 = Lookup(c);
				ret += c1 << 2 | c2 >> 4;
				state = 2;
				break;
			case 2:
				if (c == '\r' || c == '\n' || c == ' ')
					break;
				if (c == '=')
				{
					state = 20;
					break;
				}
				c3 = Lookup(c);
				ret += c2 << 4 | c3 >> 2;
				state = 3;
				break;
			case 3:
				if (c == '\r' || c == '\n' || c == ' ')
					break;
				if (c == '=')
				{
					state = 0;
					break;
				}
				c4 = Lookup(c);
				ret += c3 << 6 | c4;
				state = 0;
				break;
			case 20:// cc=
				if (c == '=')
				{
					state = 0;
					break;
				}
				throw runtime_error("Missing \"=\".");
			}
		}
		return ret;
	}

	wstring UTF8toUnicode(const string &input)
	{
		int state = 0;
		unsigned char temp;
		wchar_t wc;
		wstring wstr;
		for (size_t i=0;i<input.size();++i)
		{
			unsigned char c = input[i];
			switch (state)
			{
			case 0:
				if (c >> 4 == 14)// 1110
				{
					temp = c << 4;
					state = 1;
					break;
				}
				if (c >> 5 == 6)// 110
				{
					temp = c >> 3 & 7;// 7dec=111b
					wc = temp << 8;

					temp = c << 6;
					state = 3;
					break;
				}
				if (c >> 7 == 0) // 0
				{
					wstr += c;
					state = 0;
					break;
				}
				throw runtime_error("Decode UTF-8 error on "+to_string(i)+".");
				break;
			case 1: // 1110 xxxx
				if (c >> 6 == 2) //10
				{
					temp |= c >> 2 & 0xf;
					wc = temp << 8;

					temp = c << 6;
					state = 2;
				}
				else
					throw runtime_error("Decode UTF-8 error on " + to_string(i) + ".");
				break;
			case 2: // 1110 xxxx 10xx xxxx
				if (c >> 6 == 2) //10
				{
					temp |= c & 0x3F;
					wc |= temp;
					wstr += wc;

					state = 0;
				}
				else
					throw runtime_error("Decode UTF-8 error on " + to_string(i) + ".");
				break;
			case 3: // 110x xxxx
				if (c >> 6 == 2) //10
				{
					temp |= (c & 0x3F);
					wc |= temp;
					wstr += wc;

					state = 0;
				}
				else
					throw runtime_error("Decode UTF-8 error on " + to_string(i) + ".");
				break;
			}
		}
		return wstr;
	}

	string Unicode2GBK(const wstring &input)
	{
		int nLen = (int)input.length();

		DWORD num = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)input.c_str(), -1, NULL, 0, NULL, 0);
		string str(num, ' ');
		int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)input.c_str(), nLen, (LPSTR)str.c_str(), num, NULL, NULL);

		return str;
	}

	bool Hex2UChar(char c1, unsigned char &dst)
	{
		if (c1 >= '0' && c1 <= '9')
		{
			dst = (c1 - '0');
		}
		else
		{
			c1 = toupper(c1);
			if (c1 >= 'A'&& c1 <= 'F')
			{
				dst = (c1 - 'A' + 10);
			}
			else
				return false;
		}
		return true;
	}

	bool Hex2UChar(char c1, char c2, unsigned char &dst)
	{
		if (c1 >= '0' && c1 <= '9')
		{
			dst = (c1 - '0') << 4;
		}
		else
		{
			c1 = toupper(c1);
			if (c1 >= 'A'&& c1 <= 'F')
			{
				dst = (c1 - 'A' + 10) << 4;
			}
			else
				return false;
		}

		if (c2 >= '0' && c2 <= '9')
		{
			dst |= (c2 - '0');
		}
		else
		{
			c1 = toupper(c1);
			if (c1 >= 'A'&& c1 <= 'F')
			{
				dst |= (c1 - 'A' + 10);
			}
			else
				return false;
		}
		return true;
	}

	std::string DecodeQP(const std::string &input)
	{
		std::string ret;
		std::string temp;
		unsigned char t1, t2;
		int N = input.size();

		int state = 0;
		for (unsigned char c : input)
		{
			switch (state)
			{
			case 0:
				if (!temp.empty())
				{
					ret += temp;
					temp.clear();
				}
				if (c == '=')
				{
					state = 1;
					temp = c;
				}
				else
					ret += c;
				break;
			case 1:
				if (Hex2UChar(c, t1))
				{
					state = 2;
					temp += c;
				}
				else
				{
					state = 0;
					temp += c;
					//throw runtime_error("Decode URL error.");
				}
				break;
			case 2:
				if (Hex2UChar(c, t2))
				{
					t1 = t1 << 4 | t2;
					ret += t1;

					state = 0;
					temp.clear();
				}
				else
				{
					state = 0;
					temp += c;
				}
				break;
			}
		}
		return ret;
	}

}
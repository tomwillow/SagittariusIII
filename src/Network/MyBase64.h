#pragma once
#include <string>
#include <sstream>

namespace MyBase64
{

	inline unsigned char Lookup(unsigned char c);

	void EncodeToBase64(std::ostream& out, const std::stringstream& in, int max_line_count = INT_MAX);

	void EncodeToBase64(std::ostream& out, const std::string &in, int max_line_count = INT_MAX);

	std::string EncodeToBase64(const char* data, size_t len, int max_line_count=INT_MAX);

	std::string EncodeToBase64(std::string in, int max_line_count = INT_MAX);

	std::string DecodeBase64(const std::string &input);

	std::wstring UTF8toUnicode(const std::string &input) throw (std::runtime_error);

	std::string Unicode2GBK(const std::wstring &input) throw ();

	inline bool Hex2UChar(char c1, unsigned char &dst);

	inline bool Hex2UChar(char c1, char c2, unsigned char &dst);

	//quoted-printable
	std::string DecodeQP(const std::string &input);

}
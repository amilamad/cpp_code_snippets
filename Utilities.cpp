
#include <iostream>
#include <algorithm>
#include <functional>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

enum TextEncoding
{
	ASCII = 0,
	UTF_8,
	UTF_16,
	UTF_32
};

std::wstring convertStringToWstring(const std::string& str) 
{ 
	using namespace std;

	const ctype<wchar_t>& CType = use_facet<ctype<wchar_t> >(locale()); 
	vector<wchar_t> wideStringBuffer(str.length()); 
	CType.widen(str.data(), str.data() + str.length(), &wideStringBuffer[0]); 
	return wstring(&wideStringBuffer[0], wideStringBuffer.size()); 
} 

std::string convertWstringToString(const std::wstring& str)
{
	std::locale const loc("");
    wchar_t const* from = str.c_str();
    std::size_t const len = str.size();
    std::vector<char> buffer(len + 1);
    std::use_facet<std::ctype<wchar_t> >(loc).narrow(from, from + len, '_', &buffer[0]);
    return std::string(&buffer[0], &buffer[len]);
}


TextEncoding DetermineFileTextEncoding(const std::wstring& filePath)
{
	using namespace std;

	const short BOM_SIZE = 4;
	char buffer[BOM_SIZE];

	std::ifstream fin(filePath, ios::in | ios::binary);
	fin.read(buffer, BOM_SIZE);

	/* Text is utf-16 */
	if ((buffer[0] == '\xFF' && buffer[1] == '\xFE') || (buffer[0] == '\xFE' && buffer[1] == '\xFF'))
	{
		return TextEncoding::UTF_16;
	}

	if ((buffer[0] == '\xFF' && buffer[1] == '\xFE') && (buffer[2] == '\x00' && buffer[3] == '\x00'))
	{
		return TextEncoding::UTF_16;
	}

	if ((buffer[0] == '\x00' && buffer[1] == '\x00') && (buffer[2] == '\xFE' && buffer[3] == '\xFF'))
	{
		return TextEncoding::UTF_16;
	}

	/* Most probably text is utf-8 */
	return TextEncoding::UTF_8;
}

std::wstring ReadFileAsUnicode(const std::wstring& filePath)
{
    std::wstring ws;
	TextEncoding encoding = DetermineFileTextEncoding(filePath);

	switch(encoding)
	{
	case TextEncoding::UTF_16:
		{
			std::stringstream ss;
			std::ifstream fin(filePath);
			ss << fin.rdbuf(); // dump file contents into a stringstream
			std::string const &s = ss.str();

			if (s.size()%sizeof(wchar_t) != 0)
			{
				std::wstring errorMsg = std::wstring(L"Invalid data on file path : ") + filePath;
				throw std::exception(convertWstringToString(errorMsg).c_str());
			}

			ws.resize(s.size()/sizeof(wchar_t));
			std::memcpy(&ws[0],s.c_str(),s.size()); 
		}
		break;

	case TextEncoding::UTF_8:
		{
			std::stringstream ss;
			std::ifstream fin(filePath);
			ss << fin.rdbuf(); 
			std::string const &s = ss.str();

			ws = convertStringToWstring(s);
		}
		break;

	default:
		std::wstring errorMsg = std::wstring(L"Unsupported file format : file path : ") + filePath;
		throw std::exception(convertWstringToString(errorMsg).c_str());
		break;	
	};

	return ws;
}

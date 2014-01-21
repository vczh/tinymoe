#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "UnitTest.h"
#include "../Source/Tinymoe.h"

using namespace tinymoe;

wstring atow(const string& s)
{
	wstring buffer;
	buffer.resize(s.size() + 1);
	mbstowcs(&buffer[0], &s[0], s.size());
	return buffer.c_str();
}

string wtoa(const wstring& s)
{
	string buffer;
	buffer.resize(s.size() * 3 + 1);
	wcstombs(&buffer[0], &s[0], s.size());
	return buffer.c_str();
}

void WriteAnsiFile(string_t fileName, stringstream_t& ss)
{
	string buffer = wtoa(ss.str());

	ofstream o(fileName, ios_base::binary);
	o.write(&buffer[0], buffer.size());
}

string_t ReadAnsiFile(string_t fileName)
{
	ifstream i(fileName, ios_base::binary);
	i.seekg(0, ios_base::end);
	auto pos = i.tellg();
	i.seekg(0, ios_base::beg);

	string buffer;
	buffer.resize((size_t)pos);
	i.read(&buffer[0], pos);

#ifdef _UNICODE_TINYMOE
	return atow(buffer);
#else
	return buffer;
#endif
}

string_t GetCodeForStandardLibrary()
{
	return ReadAnsiFile(T("../Library/StandardLibrary.txt"));
}

int wmain(int argc, wchar_t* args[])
{
	_CrtDumpMemoryLeaks();
	return 0;
}
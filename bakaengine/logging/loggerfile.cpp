#include "logging/loggerfile.h"

#include <cstdarg>
#include <fstream>

#include "bakaglobal.h"

#include "game/core/utils.h"

using namespace Baka;

LoggerFile::LoggerFile(const String &filename)
{
	output_.open(filename, std::ofstream::app);

	WriteLine("------------------------------------------------------");
}

LoggerFile::~LoggerFile()
{
	output_.close();
}

Logger& LoggerFile::WriteLine(const char *format, ...)
{
	if (output_.fail())
		return *this;

	va_list vl;
	va_start(vl, format);

	char *str = new char[MAX_LINE_LENGTH];
	vsprintf(str, format, vl);
	output_ << Utils::GetTimeString() << ": " << str << std::endl;
	delete[] str;

	va_end(vl);

	return *this;
}
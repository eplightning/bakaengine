#ifndef LOGGING_LOGGERFILE_H_
#define LOGGING_LOGGERFILE_H_

#include <cstdarg>
#include <fstream>

#include "bakaglobal.h"

#include "logging/logger.h"

namespace Baka
{
	class LoggerFile : public Logger {
	public:
		static const int MAX_LINE_LENGTH = 384;

		LoggerFile(const String &filename);
		~LoggerFile();

		Logger& WriteLine(const char *format, ...);

	private:
		std::ofstream output_;
	};
} // namespace Baka

#endif
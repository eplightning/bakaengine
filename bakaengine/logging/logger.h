#ifndef LOGGING_LOGGER_H_
#define LOGGING_LOGGER_H_

#include <cstdarg>

#include "bakaglobal.h"

#ifdef _DEBUG
#define LOG_DEBUG(X, ...) Registry::Instance()->GetLogger()->WriteLine(X, ##__VA_ARGS__)
#else
#define LOG_DEBUG(X, ...) ((void) 0)
#endif

#define LOG_INFO(X, ...) Registry::Instance()->GetLogger()->WriteLine(X, ##__VA_ARGS__)

namespace Baka
{
	class Logger {
	public:
		virtual ~Logger()
		{
		}

		virtual Logger& WriteLine(const char *format, ...) = 0;
	};
} // namespace Baka

#endif
#ifndef LOGGING_LOGGERNULL_H_
#define LOGGING_LOGGERNULL_H_

#include <cstdarg>

#include "bakaglobal.h"

#include "logging/logger.h"

namespace Baka
{
	class LoggerNull : public Logger {
	public:
		~LoggerNull()
		{
		}

		Logger& WriteLine(const char *format, ...)
		{
			return *this;
		}
	};
} // namespace Baka

#endif
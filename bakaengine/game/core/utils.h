#ifndef GAME_CORE_UTILS_H_
#define GAME_CORE_UTILS_H_

#include "bakaglobal.h"

namespace Baka
{
	namespace Utils
	{
		// Platform specific stuff (utils_*.cpp)
		void PlatformInit(int argc, char *argv[]);
		String GetTimeString();
		bool FileExists(const char *filename);
		void DisplayCriticalError(const char *error);
	}
} // namespace Baka

#endif
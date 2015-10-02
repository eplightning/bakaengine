#ifndef GAME_CORE_ACTIONS_H_
#define GAME_CORE_ACTIONS_H_

#include "bakaglobal.h"

namespace Baka
{
	class Actions {
	public:
		void Execute(const String &action, bool down = true);
	};
} // namespace Baka

#endif
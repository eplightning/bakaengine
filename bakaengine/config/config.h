#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#include <unordered_map>

#include "bakaglobal.h"

namespace Baka
{
	typedef std::unordered_map<String, String> ConfigKvMap;

	class Config {
	public:
		virtual ~Config()
		{
		};

		virtual s32 GetInt(const char *section, const char *key, int def) const = 0;
		virtual double GetDouble(const char *section, const char *key, double def) const = 0;
		virtual String GetString(const char *section, const char *key, const char *def) const = 0;
		virtual bool GetBool(const char *section, const char *key, bool def) const = 0;

		virtual ConfigKvMap GetKeyMap(const char *section) const = 0;
	};
} // namespace Baka

#endif
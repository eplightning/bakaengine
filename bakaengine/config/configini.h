#ifndef CONFIG_CONFIGINI_H
#define CONFIG_CONFIGINI_H

#include "bakaglobal.h"
#include "SimpleIni.h"

#include "config/config.h"

namespace Baka
{
	class ConfigIni : public Config {
	public:
		ConfigIni(const char *filename);
		~ConfigIni();

		s32 GetInt(const char *section, const char *key, int def) const;
		double GetDouble(const char *section, const char *key, double def) const;
		String GetString(const char *section, const char *key, const char *def) const;
		bool GetBool(const char *section, const char *key, bool def) const;

		ConfigKvMap GetKeyMap(const char *section) const;
	private:
		CSimpleIniA lib_;
	};
} // namespace Baka

#endif
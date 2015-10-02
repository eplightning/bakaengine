#include "config/configini.h"

#include "bakaglobal.h"
#include "SimpleIni.h"

using namespace Baka;

ConfigIni::ConfigIni(const char *filename) : lib_(true, false, true)
{
	lib_.LoadFile(filename);
}

ConfigIni::~ConfigIni()
{
}

s32 ConfigIni::GetInt(const char *section, const char *key, int def = 0) const
{
	return static_cast<s32>(lib_.GetLongValue(section, key, def));
}

double ConfigIni::GetDouble(const char *section, const char *key, double def = 0.0) const
{
	return lib_.GetDoubleValue(section, key, def);
}

String ConfigIni::GetString(const char *section, const char *key, const char *def = nullptr) const
{
	const char *value = lib_.GetValue(section, key, def);

	if (value == nullptr)
		return String();

	return String(value);
}

bool ConfigIni::GetBool(const char *section, const char *key, bool def = false) const
{
	return lib_.GetBoolValue(section, key, def);
}

ConfigKvMap ConfigIni::GetKeyMap(const char *section) const
{
	ConfigKvMap map;

	auto libmap = lib_.GetSection(section);

	if (libmap == nullptr)
		return map;

	for (auto &x : *libmap) {
		map[x.first.pItem] = x.second;
	}

	return map;
}
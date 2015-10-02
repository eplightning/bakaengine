#include "resources/sourcefilesystem.h"

#include <fstream>

#include "bakaglobal.h"

#include "game/core/utils.h"
#include "game/core/registry.h"
#include "logging/logger.h"

using namespace Baka;

SourceFilesystem::SourceFilesystem(const char *basepath) : basepath_(basepath), cache_()
{
	if (basepath_.back() != '/')
		basepath_.push_back('/');

	LOG_INFO("Resources: Filesystem source created: %s", basepath_.c_str());
}

SourceFilesystem::~SourceFilesystem()
{
	for (auto &x : cache_) {
		if (x.second.bytes)
			delete x.second.bytes;
	}
}

bool SourceFilesystem::Exists(const char *resource)
{
	if (cache_.find(resource) != cache_.end())
		return true;

	String filepath = basepath_;
	filepath.append(resource);

	return Utils::FileExists(filepath.c_str());
}

bool SourceFilesystem::GetData(const char *resource, ResourceData *data)
{
	auto cache_entry = cache_.find(resource);

	if (cache_entry != cache_.end()) {
		data->bytes = cache_entry->second.bytes;
		data->size = cache_entry->second.size;

		return true;
	}

	String filepath = basepath_;
	filepath.append(resource);

	std::ifstream file;

	file.open(filepath, std::ifstream::in | std::ifstream::binary | std::ifstream::ate);

	if (file.fail())
		return false;

	int size = static_cast<int>(file.tellg());

	if (size <= 0)
		return false;

	char *buffer = new char[size];

	file.seekg(0, std::ifstream::beg);
	file.read(buffer, size);
	file.close();

	cache_.emplace(std::piecewise_construct, std::forward_as_tuple(resource), std::forward_as_tuple(buffer, size));
	data->chars = buffer;
	data->size = size;

	return true;
}
#ifndef RESOURCES_SOURCEFILESYSTEM_H_
#define RESOURCES_SOURCEFILESYSTEM_H_

#include <unordered_map>

#include "bakaglobal.h"

#include "resources/sourcebase.h"

namespace Baka
{
	class SourceFilesystem : public SourceBase {
	public:
		SourceFilesystem(const char *basepath);
		virtual ~SourceFilesystem();

		virtual bool Exists(const char *resource);
		virtual bool GetData(const char *resource, ResourceData *data);

	private:
		std::unordered_map<String, ResourceData> cache_;
		String basepath_;
	};
} // namespace Baka

#endif
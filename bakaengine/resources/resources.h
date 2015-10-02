#ifndef RESOURCES_RESOURCES_H_
#define RESOURCES_RESOURCES_H_

#include <vector>

#include "bakaglobal.h"

#include "resources/sourcebase.h"

namespace Baka
{
	class Resources {
	public:
		Resources();
		~Resources();

		bool Exists(const char *resource);
		ResourceData GetData(const char *resource);

		void RegisterSource(SourceBase *source);
		void UnregisterSource(SourceBase *source);

		// TODO: Streaming API
	private:
		std::vector<SourceBase*> sources_;
	};
} // namespace Baka

#endif
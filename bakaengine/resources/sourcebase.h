#ifndef RESOURCES_SOURCEBASE_H_
#define RESOURCES_SOURCEBASE_H_

#include "bakaglobal.h"

namespace Baka
{
	class ResourceData {
	public:
		ResourceData()
		{
		}

		ResourceData(const char *chars_arg, int size_arg) : chars(chars_arg), size(size_arg)
		{
		}

		union {
			const char *chars;
			const u8 *bytes;
		};

		int size;
	};

	class SourceBase {
	public:
		virtual ~SourceBase()
		{
		}

		virtual bool Exists(const char *resource) = 0;
		virtual bool GetData(const char *resource, ResourceData *data) = 0;
	};
} // namespace Baka

#endif
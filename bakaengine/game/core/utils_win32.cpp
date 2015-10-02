#ifdef _WIN32
#include "bakaglobal.h"

#include "game/core/utils.h"

#ifndef UNICODE
#error "Building for Windows without Unicode"
#endif

namespace Baka
{
	namespace Utils
	{
		void PlatformInit(int argc, char *argv[])
		{
			// working directory
			LPTSTR dir = new TCHAR[MAX_PATH];
			DWORD len = GetModuleFileName(GetModuleHandle(NULL), dir, MAX_PATH);

			if (len == 0)
				return;

			DWORD i;

			for (i = len - 1; i > 0; i--) {
				if (dir[i] == __TEXT('\\')) {
					dir[i] = __TEXT('\0');
					break;
				}
			}

			if (i > 0)
				SetCurrentDirectory(dir);

			delete[] dir;
		}

		String GetTimeString()
		{
			int size = GetTimeFormatA(LOCALE_CUSTOM_DEFAULT, NULL, NULL, NULL, NULL, 0);
			char* time = new char[size];
			GetTimeFormatA(LOCALE_CUSTOM_DEFAULT, NULL, NULL, NULL, time, size);

			String str(time);

			delete[] time;

			return str;
		}

		bool FileExists(const char *filename)
		{
			TCHAR *path = new TCHAR[MAX_PATH];

			DWORD path_length = GetCurrentDirectory(MAX_PATH, path);

			DWORD length = MultiByteToWideChar(CP_UTF8, 0, filename, -1, NULL, 0);

			if (length == 0)
				return false;

			TCHAR *converted_filename = new TCHAR[length];
			length = MultiByteToWideChar(CP_UTF8, 0, filename, -1, converted_filename, length);

			if (length == 0)
				return false;

			path[path_length] = __TEXT('\\');
			path[path_length + 1] = __TEXT('\0');

			wcsncat(path, converted_filename, length);

			delete[] converted_filename;

			DWORD attributes = GetFileAttributes(path);

			delete[] path;

			return (attributes != 0xFFFFFFFF && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
		}

		void DisplayCriticalError(const char *message)
		{
			DWORD length = MultiByteToWideChar(CP_UTF8, 0, message, -1, NULL, 0);

			if (length == 0)
				return;

			TCHAR *converted_message = new TCHAR[length];
			length = MultiByteToWideChar(CP_UTF8, 0, message, -1, converted_message, length);

			if (length == 0)
				return;

			MessageBox(GetActiveWindow(), converted_message, __TEXT("Critical Error"), MB_OK | MB_ICONERROR);

			delete[] converted_message;
		}
	} // namespace Utils
} // namespace Baka
#endif
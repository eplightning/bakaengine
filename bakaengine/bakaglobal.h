#ifndef BAKAGLOBAL_H_
#define BAKAGLOBAL_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include <climits>
#include <functional>
#include <string>
#include <memory>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#define BIND_METHOD_CALL(F, O) std::bind(&F, O, std::placeholders::_1)

namespace Baka
{
	// Info
	static const char *ENGINE_VERSION = "0.1";
	static const char *ENGINE_CODENAME = "Minami";
	static const char *ENGINE_DEFAULT_WINDOW_TITLE = "Baka Engine 0.1";

	// STL
	typedef std::string String;
	template<class T> using UniquePtr = std::unique_ptr < T > ;
	template<class T> using SharedPtr = std::shared_ptr < T > ;
	template<class T> using WeakPtr = std::weak_ptr < T > ;

	// GLM
	typedef glm::mat4 Mat4;
	typedef glm::vec4 Vec4;
	typedef glm::vec3 Vec3;
	typedef glm::vec2 Vec2;

	// uint
	typedef unsigned int uint;

	// 8-bit integer
#if UCHAR_MAX == 255
	typedef unsigned char u8;
	typedef signed char s8;
#else
#error "No 8-bit int type found"
#endif

	// 16-bit integer
#if USHRT_MAX == 65535
	typedef unsigned short u16;
	typedef signed short s16;
#elif UINT_MAX == 65535
	typedef unsigned int u16;
	typedef signed int s16;
#else
#error "No 16-bit int type found"
#endif

	// 32-bit integer
#if UINT_MAX == 4294967295
	typedef unsigned int u32;
	typedef signed int s32;
#elif ULONG_MAX == 4294967295
	typedef unsigned long u32;
	typedef signed long s32;
#elif USHRT_MAX == 4294967295
	typedef unsigned short u32;
	typedef signed short s32;
#else
#error "No 32-bit int type found"
#endif

	// 64-bit integer
#if ULONG_MAX == 18446744073709551615
	typedef unsigned long u64;
	typedef signed long s64;
#elif ULLONG_MAX == 18446744073709551615
	typedef unsigned long long u64;
	typedef signed long long s64;
#elif UINT_MAX == 18446744073709551615
	typedef unsigned int u64;
	typedef signed int s64;
#else
#error "No 64-bit int type found"
#endif
}

#endif
#ifndef GRAPHICS_OBJCACHE_H_
#define GRAPHICS_OBJCACHE_H_

#include <unordered_map>

#include "bakaglobal.h"
#include <GL/glew.h>

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"

namespace Baka
{
	typedef std::unordered_map<String, SharedPtr<Texture>> ObjCacheTextures;
	typedef std::unordered_map<String, SharedPtr<Shader>> ObjCacheShaders;
	typedef std::unordered_map<String, SharedPtr<Mesh>> ObjCacheMeshes;

	class ObjCache {
	public:
		void Clear();
		SharedPtr<Mesh> GetMesh(const String &resource);
		SharedPtr<Shader> GetShader(const String &resource);
		SharedPtr<Texture> GetTexture(const String &resource);

		bool IsShaderLoaded(const String &resource) const;
		void SetCustomShader(const String &name, SharedPtr<Shader> shader);

	private:
		ObjCacheTextures textures_;
		ObjCacheShaders shaders_;
		ObjCacheMeshes meshes_;
	};
} // namespace Baka

#endif
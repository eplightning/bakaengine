#include "graphics/objcache.h"

#include "bakaglobal.h"
#include <GL/glew.h>

using namespace Baka;

void ObjCache::Clear()
{
	meshes_.clear();
	textures_.clear();
	shaders_.clear();
}

SharedPtr<Mesh> ObjCache::GetMesh(const String &resource)
{
	auto it = meshes_.find(resource);

	if (it == meshes_.end()) {
		Mesh *mesh = new Mesh(resource.c_str(), this);

		it = meshes_.emplace(std::piecewise_construct, std::forward_as_tuple(resource), std::forward_as_tuple(mesh)).first;
	}

	return it->second;
}

SharedPtr<Shader> ObjCache::GetShader(const String &resource)
{
	auto it = shaders_.find(resource);

	if (it == shaders_.end()) {
		String fshader(resource);
		fshader += ".frag";

		Shader *sh = new Shader(resource.c_str(), fshader.c_str());

		it = shaders_.emplace(std::piecewise_construct, std::forward_as_tuple(resource), std::forward_as_tuple(sh)).first;
	}

	return it->second;
}

SharedPtr<Texture> ObjCache::GetTexture(const String &resource)
{
	auto it = textures_.find(resource);

	if (it == textures_.end()) {
		Texture *tex = new Texture(resource.c_str());

		it = textures_.emplace(std::piecewise_construct, std::forward_as_tuple(resource), std::forward_as_tuple(tex)).first;
	}

	return it->second;
}

bool ObjCache::IsShaderLoaded(const String& resource) const
{
	return shaders_.find(resource) != shaders_.end();
}

void ObjCache::SetCustomShader(const String& name, SharedPtr<Shader> shader)
{
	shaders_.emplace(name, shader);
}
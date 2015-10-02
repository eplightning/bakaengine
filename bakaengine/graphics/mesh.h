#ifndef GRAPHICS_MESH_H_
#define GRAPHICS_MESH_H_

#include <vector>

#include "bakaglobal.h"
#include <GL/glew.h>
#include <assimp/scene.h>

#include "graphics/texture.h"

namespace Baka
{
	class ObjCache;

	struct MeshEntry {
		uint material_index;
		uint vertex_base;
		uint vertex_count;
		uint index_base;
	};

	struct MeshMaterial {
		SharedPtr<Texture> texture_diffuse;
		SharedPtr<Texture> texture_specular;
		SharedPtr<Texture> texture_ambient;
		float shininess;
	};

	class MeshOpenGLData {
	public:
		~MeshOpenGLData();

		GLuint vao;
		GLuint vbo[4];
	};

	typedef std::vector<MeshMaterial> MeshMaterials;
	typedef std::vector<MeshEntry> MeshEntries;

	class Mesh {
	public:
		explicit Mesh(const char *resource, ObjCache *cache = nullptr);
		Mesh(const Mesh &other);
		~Mesh();

		const MeshEntries& GetEntries() const;
		const MeshMaterial& GetMaterial(uint index) const;
		MeshMaterial& GetMaterial(uint index);
		GLuint GetVAO() const;
		const Vec3* GetBounds() const;

	protected:
		void LoadFromResource(const char *resource, ObjCache *cache);

		void LoadEntries(const aiScene *scene);
		void LoadEntry(const aiMesh *mesh, std::vector<Vec3> &positions, std::vector<Vec3> &normals, std::vector<Vec2> &texcoords, std::vector<uint> &indices);
		void LoadMaterials(const aiScene *scene, ObjCache *cache);
		void LoadTexture(const char *key, uint type, uint idx, const aiMaterial *mat, SharedPtr<Texture> &ptr, ObjCache *cache);

		SharedPtr<MeshOpenGLData> gl_;
		MeshMaterials materials_;
		MeshEntries entries_;
		Vec3 bounds_[8];
		Vec3 minpos_;
		Vec3 maxpos_;
	};
} // namespace Baka

#endif
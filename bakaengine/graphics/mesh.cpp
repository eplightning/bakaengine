#include "graphics/mesh.h"

#include <cfloat>

#include "bakaglobal.h"
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "game/core/registry.h"
#include "resources/resources.h"
#include "logging/logger.h"
#include "graphics/objcache.h"

using namespace Baka;

MeshOpenGLData::~MeshOpenGLData()
{
	glDeleteBuffers(4, vbo);
	glDeleteVertexArrays(1, &vao);
}

Mesh::Mesh(const char *resource, ObjCache *cache)
{
	LoadFromResource(resource, cache);
}

Mesh::Mesh(const Mesh& other)
{
	gl_ = other.gl_;
	entries_ = other.entries_;
	materials_ = other.materials_;

	for (int i = 0; i < 8; i++) {
		bounds_[i] = other.bounds_[i];
	}

	minpos_ = other.minpos_;
	maxpos_ = other.maxpos_;
}

Mesh::~Mesh()
{
}

const MeshEntries& Mesh::GetEntries() const
{
	return entries_;
}

const MeshMaterial& Mesh::GetMaterial(uint index) const
{
	return materials_[index];
}

MeshMaterial& Mesh::GetMaterial(uint index)
{
	return materials_[index];
}

void Mesh::LoadFromResource(const char *resource, ObjCache *cache)
{
	ResourceData res = Registry::Instance()->GetResources()->GetData(resource);

	if (res.size == 0) {
		LOG_INFO("Model file `%s` not found in resources", resource);

		throw std::logic_error("Model file not found");
	}

	Assimp::Importer import;

	const aiScene *scene = import.ReadFileFromMemory(res.bytes, res.size,
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (scene == nullptr) {
		LOG_INFO("Model file `%s` could not be imported: %s", resource, import.GetErrorString());

		throw std::logic_error("Model file import error");
	}

	LoadEntries(scene);
	LoadMaterials(scene, cache);
}

void Mesh::LoadEntries(const aiScene *scene)
{
	MeshOpenGLData *data = new MeshOpenGLData;
	gl_.reset(data);

	glGenVertexArrays(1, &data->vao);
	glBindVertexArray(data->vao);
	glGenBuffers(4, data->vbo);

	entries_.resize(scene->mNumMeshes);

	uint base_index = 0;
	uint base_vertex = 0;

	for (uint i = 0; i < scene->mNumMeshes; i++) {
		const aiMesh *mesh = scene->mMeshes[i];

		entries_[i].material_index = mesh->mMaterialIndex;
		entries_[i].vertex_count = mesh->mNumFaces * 3; // aiProcess_Triangulate gwarantuje nam trójkąty
		entries_[i].index_base = base_index;
		entries_[i].vertex_base = base_vertex;

		base_index += entries_[i].vertex_count;
		base_vertex += mesh->mNumVertices;
	}

	std::vector<Vec3> positions;
	std::vector<Vec3> normals;
	std::vector<Vec2> texcoords;
	std::vector<uint> indices;

	positions.reserve(base_vertex);
	normals.reserve(base_vertex);
	texcoords.reserve(base_vertex);
	indices.reserve(base_index);

	minpos_ = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	maxpos_ = Vec3(FLT_MIN, FLT_MIN, FLT_MIN);

	for (uint i = 0; i < scene->mNumMeshes; i++) {
		LoadEntry(scene->mMeshes[i], positions, normals, texcoords, indices);
	}

	// bounding box from minpos, maxpos
	bounds_[0] = Vec3(minpos_.x, minpos_.y, minpos_.z); // xyz
	bounds_[1] = Vec3(maxpos_.x, minpos_.y, minpos_.z); // Xyz
	bounds_[2] = Vec3(minpos_.x, maxpos_.y, minpos_.z); // xYz
	bounds_[3] = Vec3(maxpos_.x, maxpos_.y, minpos_.z); // XYz
	bounds_[4] = Vec3(minpos_.x, minpos_.y, maxpos_.z); // xyZ
	bounds_[5] = Vec3(maxpos_.x, minpos_.y, maxpos_.z); // XyZ
	bounds_[6] = Vec3(minpos_.x, maxpos_.y, maxpos_.z); // xYZ
	bounds_[7] = Vec3(maxpos_.x, maxpos_.y, maxpos_.z); // XYZ

	// positions
	glBindBuffer(GL_ARRAY_BUFFER, data->vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// texcoords
	glBindBuffer(GL_ARRAY_BUFFER, data->vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords[0]) * texcoords.size(), &texcoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// normals
	glBindBuffer(GL_ARRAY_BUFFER, data->vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::LoadEntry(const aiMesh* mesh, std::vector<Vec3>& positions, std::vector<Vec3>& normals, std::vector<Vec2>& texcoords, std::vector<uint>& indices)
{
	const aiVector3D zero(0.f, 0.f, 0.f);

	for (uint i = 0; i < mesh->mNumVertices; i++) {
		const aiVector3D *pos = &mesh->mVertices[i];
		const aiVector3D *nor = &mesh->mNormals[i];
		const aiVector3D *tex = mesh->HasTextureCoords(0) ? &mesh->mTextureCoords[0][i] : &zero;

		positions.emplace_back(pos->x, pos->y, pos->z);
		normals.emplace_back(nor->x, nor->y, nor->z);
		texcoords.emplace_back(tex->x, tex->y);

		// najmniejsze/największe xyz
		if (minpos_.x > pos->x) minpos_.x = pos->x;
		if (minpos_.y > pos->y) minpos_.y = pos->y;
		if (minpos_.z > pos->z) minpos_.z = pos->z;
		if (maxpos_.x < pos->x) maxpos_.x = pos->x;
		if (maxpos_.y < pos->y) maxpos_.y = pos->y;
		if (maxpos_.z < pos->z) maxpos_.z = pos->z;
	}

	for (uint i = 0; i < mesh->mNumFaces; i++) {
		const aiFace &face = mesh->mFaces[i];

		indices.emplace_back(face.mIndices[0]);
		indices.emplace_back(face.mIndices[1]);
		indices.emplace_back(face.mIndices[2]);
	}
}

void Mesh::LoadMaterials(const aiScene* scene, ObjCache *cache)
{
	materials_.resize(scene->mNumMaterials);

	for (uint i = 0; i < scene->mNumMaterials; i++) {
		LoadTexture(AI_MATKEY_TEXTURE_AMBIENT(0), scene->mMaterials[i], materials_[i].texture_ambient, cache);
		LoadTexture(AI_MATKEY_TEXTURE_DIFFUSE(0), scene->mMaterials[i], materials_[i].texture_diffuse, cache);
		LoadTexture(AI_MATKEY_TEXTURE_SPECULAR(0), scene->mMaterials[i], materials_[i].texture_specular, cache);

		materials_[i].shininess = 16;
		scene->mMaterials[i]->Get(AI_MATKEY_SHININESS, materials_[i].shininess);
	}
}

void Mesh::LoadTexture(const char* key, uint type, uint idx, const aiMaterial* mat, SharedPtr<Texture>& ptr, ObjCache *cache)
{
	aiString str;
	aiReturn ret = mat->Get(key, type, idx, str);

	if (ret == AI_SUCCESS) {
		if (cache == nullptr) {
			ptr.reset(new Texture(str.data));
		} else {
			ptr = cache->GetTexture(str.data);
		}
	}
}

GLuint Mesh::GetVAO() const
{
	return gl_->vao;
}

const Vec3* Mesh::GetBounds() const
{
	return bounds_;
}
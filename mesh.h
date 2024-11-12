#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/scene.h>
#include <iostream>
#include <vector>
#include"texture.h"
#include"math3d.h"
#include "Technique.h"
#include "LightingTechinque.h"
#include "Pipeline.h"
#include "Camera.h"

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;
	Vector3f m_normal;
	Vertex(){}
	Vertex(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = normal;
	}
};

class Mesh
{
public:
	Mesh();
	~Mesh();

	bool LoadMesh(const std::string& Filename);
	void Render(LightingTechinque* m_pEffect);

private:
#define INVALID_MATERIAL 0XFFFFFFFF

	struct MeshEntry
	{
		MeshEntry();
		~MeshEntry();

		void Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices);

		GLuint VB;
		GLuint IB;
		GLuint VA;

		unsigned int NumIndices;
		unsigned int MaterialIndex;
	};

	void processNode(const aiNode* node, const aiScene* scene);
	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	void GetTexture(aiTextureType TextureType, const aiMaterial* pMaterial, std::string Dir, int i, bool Ret);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

	Vector3f m_center;
	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;
};
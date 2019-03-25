#include "Model.h"



Model::Model()
{
}


void Model::LoadModel(const std::string& filename)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		printf("Model %s failed to load: %s", filename, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene);
}


void Model::RenderModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		unsigned int materialIndex = meshToTex[i];

		if (materialIndex < textureList.size() && textureList[materialIndex])
		{
			textureList[materialIndex]->UseTexture();
		}

		meshList[i]->RenderMesh();
	}
}


void Model::ClearModel()
{
	for (size_t i = 0; i < meshList.size(); i++)
	{
		if (meshList[i])
		{
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++)
	{
		if (textureList[i])
		{
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}


void Model::LoadNode(aiNode *node, const aiScene *scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	// Recursive load on child nodes until there are no children
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}


void Model::LoadMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		// insert vertex coordinates
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,  mesh->mVertices[i].y,  mesh->mVertices[i].z });
		
		// insert texture coordinates
		if (mesh->mTextureCoords[0]) // if there are texture coordinates, hences the texture exists
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x,  mesh->mTextureCoords[0][i].y });
		}
		else
		{
			vertices.insert(vertices.end(), { 0.0f,  0.0f });
		}
		
		// insert normals { normals are reversed because the normals in the diffuse factor of the fragment shader are not negative (typically they are)}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x,  --mesh->mNormals[i].y,  -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Create the new mesh
	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());

	// Add the new mesh to list and add its texture to meshToTex
	meshList.push_back(newMesh);
	meshToTex.push_back(mesh->mMaterialIndex);
}


void Model::LoadMaterials(const aiScene *scene)
{
	textureList.resize(scene->mNumMaterials);
	
	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* material = scene->mMaterials[i];

		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			// out parameter
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				// get filename without absolute path
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				// set the new path for the way our folder layout
				std::string texPath = std::string("Textures/") + filename;

				textureList[i] = new Texture(texPath.c_str());

				if (!textureList[i]->LoadTexture())
				{
					printf("Failed to load textur at: %s\n", texPath);
					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		if (!textureList[i])
		{
			textureList[i] = new Texture("Textures/plain.png");
			textureList[i]->LoadTexture();
		}
	}
}


Model::~Model()
{
}

#include "FracturePCH.h"
#include "MeshLoader.h"
#include "Rendering/Mesh.h"
#include "Serialisation/Serialiser.h"
#include "Rendering/GraphicsDevice.h"
#include <thread>
#include <future>

const uint32_t Fracture::MeshLoader::s_MeshImportFlags;

Fracture::MeshLoader::MeshLoader()
{
}

Fracture::MeshRegistry Fracture::MeshLoader::ImportStaticMesh(const std::string& path, const std::string& out_path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, s_MeshImportFlags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		FRACTURE_ERROR("ERROR::ASSIMP:: {}",importer.GetErrorString());
		return MeshRegistry();
	}
	
	uint32_t vertexCount = 0;
	uint32_t indexCount = 0;
	uint32_t faceCount = 0;
	std::vector<glm::vec3> mPositions;
	std::vector<glm::vec2> mUVs;
	std::vector<glm::vec3> mNormals;
	std::vector<SubMesh> m_submeshes;
	std::vector<unsigned int> indices;
	std::vector<MeshTriangle> triangles;

	m_submeshes.reserve(scene->mNumMeshes);
	{		
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{		
			{
				auto& node = scene->mRootNode;
				aiMesh* mesh = scene->mMeshes[i];
				aiString meshname = node->mName;

				
				auto submesh = SubMesh();
				submesh.BaseVertex = vertexCount;
				submesh.BaseIndex = indexCount;
				submesh.MaterialIndex = mesh->mMaterialIndex;
				submesh.IndexCount = mesh->mNumFaces * 3;
				submesh.MeshName = mesh->mName.C_Str();
				vertexCount += mesh->mNumVertices;
				faceCount += mesh->mNumFaces;
				uint32_t submeshIndexCount = (mesh->mNumFaces * 3);
				indexCount += submeshIndexCount;

				for (unsigned int i = 0; i < mesh->mNumVertices; i++)
				{
					{
						glm::vec3 vector;
						vector.x = mesh->mVertices[i].x;
						vector.y = mesh->mVertices[i].y;
						vector.z = mesh->mVertices[i].z;
						mPositions.push_back(vector);
					}
					{
						glm::vec3 vector;
						vector.x = mesh->mNormals[i].x;
						vector.y = mesh->mNormals[i].y;
						vector.z = mesh->mNormals[i].z;
						mNormals.push_back(vector);
					}
					if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
					{
						glm::vec2 vec;
						vec.x = mesh->mTextureCoords[0][i].x;
						vec.y = mesh->mTextureCoords[0][i].y;
						mUVs.push_back(vec);
					}
					else
					{
						mUVs.push_back(glm::vec2(0.0f, 0.0f));
					}
				}


				//glm::vec3 min = { mesh->mAABB.mMin.x,mesh->mAABB.mMin.y,mesh->mAABB.mMin.z };
				//glm::vec3 max = { mesh->mAABB.mMax.x,mesh->mAABB.mMax.y,mesh->mAABB.mMax.z };
				//submesh.aabb = AABB(min, max);
				//process aabb
				/*
				for (size_t i = 0; i < mPositions.size(); i++)
				{

					glm::vec3 pos = { mPositions[i] };
					submesh.aabb.min.x = glm::min(pos.x, submesh.aabb.min.x);
					submesh.aabb.min.y = glm::min(pos.y, submesh.aabb.min.y);
					submesh.aabb.min.z = glm::min(pos.z, submesh.aabb.min.z);
					submesh.aabb.max.x = glm::max(pos.x, submesh.aabb.max.x);
					submesh.aabb.max.y = glm::max(pos.y, submesh.aabb.max.y);
					submesh.aabb.max.z = glm::max(pos.z, submesh.aabb.max.z);
				}
				*/
				for (unsigned int i = 0; i < mesh->mNumFaces; i++)
				{
					aiFace face = mesh->mFaces[i];
					for (unsigned int j = 0; j < face.mNumIndices; j++)
						indices.push_back(face.mIndices[j]);
				}
				/*
				Meshaabb = Meshaabb.Merge(submesh.aabb);
				*/
				m_submeshes.push_back(submesh);


				//Triangle Cache for Ray Picking
				for (unsigned int x = 0; x < mesh->mNumFaces; x++)
				{
					MeshTriangle triangle;
					glm::vec3 vector;

					vector.x = mesh->mVertices[mesh->mFaces[x].mIndices[0]].x;
					vector.y = mesh->mVertices[mesh->mFaces[x].mIndices[0]].y;
					vector.z = mesh->mVertices[mesh->mFaces[x].mIndices[0]].z;
					triangle.v1 = vector;

					vector.x = mesh->mVertices[mesh->mFaces[x].mIndices[1]].x;
					vector.y = mesh->mVertices[mesh->mFaces[x].mIndices[1]].y;
					vector.z = mesh->mVertices[mesh->mFaces[x].mIndices[1]].z;
					triangle.v2 = vector;

					vector.x = mesh->mVertices[mesh->mFaces[x].mIndices[2]].x;
					vector.y = mesh->mVertices[mesh->mFaces[x].mIndices[2]].y;
					vector.z = mesh->mVertices[mesh->mFaces[x].mIndices[2]].z;
					triangle.v3 = vector;


					// compute plane's normal
					glm::vec3 v0v1 = triangle.v2 - triangle.v1;
					glm::vec3 v0v2 = triangle.v3 - triangle.v1;
					// no need to normalize
					triangle.Normal = glm::cross(v0v1, v0v2); // N 

					triangles.push_back(triangle);
				}
			}				
		}	
	}	
		
	//staticmesh->aabb = Meshaabb;
	//staticmesh->bvhRoot.SetAABB(Meshaabb);
	//staticmesh->bvhRoot.PushTriangles(triangles);
	//staticmesh->bvhRoot.Split();
	//staticmesh->bvhRoot.Shake();

	//for (unsigned int i = 0; i < m_submeshes.size(); i++)
	//{
	//	staticmesh->aabb.Merge(m_submeshes[i].aabb);
	//}

	//auto meshwriter = MeshSerialiser(*staticmesh.get(), IOMode::Save, SerialiseFormat::Binary);
	//meshwriter.BeginMesh();
	//meshwriter.VEC3_VertexData("Positions", mPositions);;
	//meshwriter.VEC3_VertexData("Normals", mNormals);
	//meshwriter.VEC2_VertexData("UVs", mUVs);
	//meshwriter.Property("Indicies", indices);
	//meshwriter.SubMeshes(m_submeshes);
	//meshwriter.EndMesh();
	//std::string name = scene->mMeshes[0]->mName.C_Str();
	//meshwriter.Save("Content\\meshes\\" + name  + ".mesh");
	
	ISerialiser modelSerialiser = ISerialiser(Fracture::ISerialiser::IOMode::Save, Fracture::ISerialiser::SerialiseFormat::Binary);
	modelSerialiser.BeginStruct("Mesh");
	modelSerialiser.Property("ID", UUID());
	modelSerialiser.Property("Positions", mPositions);
	modelSerialiser.Property("Normals", mNormals);
	modelSerialiser.Property("UVs", mUVs);
	modelSerialiser.Property("Indicies", indices);
	modelSerialiser.Property("SubMeshes", m_submeshes);
	modelSerialiser.EndStruct();

	std::string file_name = path.substr(path.find_last_of("/\\") + 1);
	std::string::size_type const p(file_name.find_last_of('.'));
	std::string file_without_extension = file_name.substr(0, p);


	std::string Output_path = out_path+"/meshes/ " + file_without_extension + ".mesh";
	modelSerialiser.Save(Output_path);

	MeshRegistry reg; 
	reg.ID = UUID();
	reg.meshType = Fracture::MeshRegistry::MeshType::Static;
	reg.Name = file_without_extension;
	reg.Path = Output_path;
	return reg;
}

std::shared_ptr<Fracture::StaticMesh> Fracture::MeshLoader::LoadStaticMesh(const std::string& path)
{
	ISerialiser modelSerialiser = ISerialiser(Fracture::ISerialiser::IOMode::Open, Fracture::ISerialiser::SerialiseFormat::Binary);
	modelSerialiser.Open(path);

	if (modelSerialiser.BeginStruct("Mesh"))
	{
		std::shared_ptr<Fracture::StaticMesh> mesh = std::make_shared<Fracture::StaticMesh>(modelSerialiser.ID("ID"));
		auto position_result = std::async(std::launch::async, [&]() { return modelSerialiser.VEC3_VECTOR("Positions"); });
		auto normal_result = std::async(std::launch::async, [&]() { return modelSerialiser.VEC3_VECTOR("Normals"); });
		auto UV_result = std::async(std::launch::async, [&]() { return modelSerialiser.VEC2_VECTOR("UVs"); });
		auto Indicies_result = std::async(std::launch::async, [&]() { return modelSerialiser.UINT_VECTOR("Indicies"); });
		auto SubMeshes_result = std::async(std::launch::async, [&]() { return modelSerialiser.SUBMESH_VECTOR("SubMeshes"); });

		mesh->mPositions = position_result.get();
		mesh->mNormals = normal_result.get();
		mesh->mUVs = UV_result.get();
		mesh->Indices = Indicies_result.get();
		mesh->SubMeshes = SubMeshes_result.get();		

		modelSerialiser.EndStruct();
		return mesh;
	}

	


}


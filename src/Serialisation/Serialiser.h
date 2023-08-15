#pragma once
#ifndef SERIALISER_H
#define SERIALISER_H

#include "Common/UUID.h"
#include "nlohmann/json.hpp"
#include "Rendering/Mesh.h"
#include "Assets/AssetManager.h"

namespace nlohmann
{
	using json = nlohmann::json;

	inline void  to_json(json& j, const glm::vec2& vec)
	{
		j = json{ {"x", vec.x}, {"y", vec.y} };
	}
	inline void  to_json(json& j, const glm::vec3& vec)
	{
		j = json{ {"x", vec.x}, {"y", vec.y},{"z", vec.z} };
	}
	inline void  to_json(json& j, const glm::vec4& vec)
	{
		j = json{ {"x", vec.x}, {"y", vec.y},{"z", vec.z},{"w", vec.w} };
	}
	inline void  to_json(json& j, const glm::quat& vec)
	{
		j = json{ {"x", vec.x}, {"y", vec.y},{"z", vec.z},{"w", vec.w}};
	}
	inline void  to_json(json& j, const Fracture::SubMesh& sub)
	{
		j = json{
			{"BaseIndex", sub.BaseIndex},
			{"BaseVertex", sub.BaseVertex},
			{"IndexCount", sub.IndexCount},
			{"MaterialIndex", sub.MaterialIndex},
		};
	}
	inline void  to_json(json& j, const Fracture::MeshRegistry& reg)
	{
		j = json{
			{"ID", (uint32_t)reg.ID},
			{"Name", reg.Name},
			{"Path", reg.Path},
			{"Type", reg.meshType}
		};
	}	
	inline void  to_json(json& j, const Fracture::ShaderRegistry& reg)
	{
		j = json{
			{"ID", (uint32_t)reg.ID},
			{"Name", reg.Name},
			{"Vertex_Path", reg.Vertex_Path},
			{"Frag_Path", reg.Fragment_Path},
			{"Geo_Path", reg.Geometry_Path},
			{"Comp_Path", reg.Compute_Path},
			{"TesControl_Path", reg.TessalationControl_Path},
			{"TesEval_Path", reg.TessalationEval_Path}
		};
	}
	inline void  to_json(json& j, const Fracture::SceneRegistry& reg)
	{
		j = json{
			{"ID", (uint32_t)reg.ID},
			{"Name", reg.Name},
			{"Path", reg.Path}
		};
	}
	inline void  to_json(json& j, const Fracture::MaterialRegistry& reg)
	{
		j = json{
			{"ID", (uint32_t)reg.ID},
			{"Name", reg.Name},
			{"Path", reg.Path}
		};
	}
	inline void  to_json(json& j, const Fracture::TextureRegistry& reg)
	{
		j = json{
			{"ID", (uint32_t)reg.ID},
			{"Name", reg.Name},
			{"Path", reg.Path}
		};
	}
	inline void  to_json(json& j, const Fracture::LuaScriptRegistry& reg)
	{
		j = json{
			{"ID", (uint32_t)reg.ID},
			{"Name", reg.Name},
			{"Path", reg.Path}
		};
	}
	inline void  to_json(json& j, const Fracture::AnimationClipRegistry& reg)
	{
		j = json{
			{"ID", (uint32_t)reg.ID},
			{"Name", reg.Name},
			{"Path", reg.Path}
		};
	}


	

	inline void from_json(const json& j, Fracture::ShaderRegistry& reg) {
		uint32_t id;
		j.at("ID").get_to(id);
		j.at("Name").get_to(reg.Name);
		j.at("Vertex_Path").get_to(reg.Vertex_Path);
		j.at("Frag_Path").get_to(reg.Fragment_Path);
		j.at("Geo_Path").get_to(reg.Geometry_Path);
		j.at("Comp_Path").get_to(reg.Compute_Path);
		j.at("TesControl_Path").get_to(reg.TessalationControl_Path);
		j.at("TesEval_Path").get_to(reg.TessalationEval_Path);
		reg.ID = id;
	}
	inline void from_json(const json& j, Fracture::MeshRegistry& reg) {
		uint32_t id;
		j.at("ID").get_to(id);
		j.at("Name").get_to(reg.Name);
		j.at("Path").get_to(reg.Path);
		j.at("Type").get_to(reg.meshType);
		reg.ID = id;
	}
	inline void from_json(const json& j, Fracture::SceneRegistry& reg) {
		uint32_t id;
		j.at("ID").get_to(id);
		j.at("Name").get_to(reg.Name);
		j.at("Path").get_to(reg.Path);
		reg.ID = id;
	}
	inline void from_json(const json& j, Fracture::SubMesh& sub) {
		j.at("BaseIndex").get_to(sub.BaseIndex);
		j.at("BaseVertex").get_to(sub.BaseVertex);
		j.at("IndexCount").get_to(sub.IndexCount);
		j.at("MaterialIndex").get_to(sub.MaterialIndex);
	}
	inline void from_json(const json& j, Fracture::MaterialRegistry& reg) {
		uint32_t id;
		j.at("ID").get_to(id);
		j.at("Name").get_to(reg.Name);
		j.at("Path").get_to(reg.Path);
		reg.ID = id;
	}
	inline void from_json(const json& j, Fracture::TextureRegistry& reg) {
		uint32_t id;
		j.at("ID").get_to(id);
		j.at("Name").get_to(reg.Name);
		j.at("Path").get_to(reg.Path);
		reg.ID = id;
	}
	inline void from_json(const json& j, Fracture::LuaScriptRegistry& reg) {
		uint32_t id;
		j.at("ID").get_to(id);
		j.at("Name").get_to(reg.Name);
		j.at("Path").get_to(reg.Path);
		reg.ID = id;
	}
	inline void from_json(const json& j, Fracture::AnimationClipRegistry& reg) {
		uint32_t id;
		j.at("ID").get_to(id);
		j.at("Name").get_to(reg.Name);
		j.at("Path").get_to(reg.Path);
		reg.ID = id;
	}
	inline void from_json(const json& j, glm::vec2& vec) {
		j.at("x").get_to(vec.x);
		j.at("y").get_to(vec.y);
	}
	inline void from_json(const json& j, glm::vec3& vec) {
		j.at("x").get_to(vec.x);
		j.at("y").get_to(vec.y);
		j.at("z").get_to(vec.z);
	}
	inline void from_json(const json& j, glm::vec4& vec) {
		j.at("x").get_to(vec.x);
		j.at("y").get_to(vec.y);
		j.at("z").get_to(vec.z);
		j.at("w").get_to(vec.w);
	}
	inline void from_json(const json& j, glm::quat& vec) {
		j.at("x").get_to(vec.x);
		j.at("y").get_to(vec.y);
		j.at("z").get_to(vec.z);
		j.at("w").get_to(vec.w);
	}
}

namespace Fracture
{
	struct SubMesh;
	
	using namespace nlohmann;

	class ISerialiser
	{
	public:

		enum class IOMode
		{
			Save,
			Open
		};

		enum class SerialiseFormat
		{
			Json,
			Binary
		};

		ISerialiser(IOMode mode, SerialiseFormat format);

		void Property(const std::string& name, uint32_t value);
		void Property(const std::string& name, int value);
		void Property(const std::string& name, bool  value);
		void Property(const std::string& name, Fracture::UUID value);
		void Property(const std::string& name, const float& value);
		void Property(const std::string& name, const char* value);
		void Property(const std::string& name, const std::string& value);
		void Property(const std::string& name, const Fracture::MeshRegistry& value);
		void Property(const std::string& name, const Fracture::ShaderRegistry& value);
		void Property(const std::string& name, const Fracture::SceneRegistry& value);
		void Property(const std::string& name, const Fracture::TextureRegistry& value);
		void Property(const std::string& name, const Fracture::MaterialRegistry& value);
		void Property(const std::string& name, const Fracture::LuaScriptRegistry& value);
		void Property(const std::string& name, const Fracture::AnimationClipRegistry& value);
		void Property(const std::string& name, const glm::vec2& value);
		void Property(const std::string& name, const glm::vec3& value);
		void Property(const std::string& name, const glm::vec4& value);
		void Property(const std::string& name, const glm::quat& value);
		void Property(const std::string& name, const std::vector<unsigned int>& value);
		void Property(const std::string& name, const std::vector<int>& value);
		void PropertyI(const std::string& name, const std::vector<uint32_t>& value);
		void Property(const std::string& name, const std::vector<std::string>& values);
		void Property(const std::string& name, const std::vector<glm::vec2>& value);
		void Property(const std::string& name, const std::vector<glm::vec3>& value);
		void Property(const std::string& name, const std::vector<glm::vec4>& value);
		void Property(const std::string& name, const std::vector<Fracture::SubMesh>& value);
		void Property(const std::string& name, const std::vector<unsigned char>& value);
		void Property(const std::string& name, const std::vector<float>& value);


		void Save(const std::string& path);
		bool Open(const std::string& path);

		bool BeginStruct(const std::string& name);
		void EndStruct();

		bool BeginCollection(const std::string& name);
		void NextInCollection();
		void EndCollection();

		uint32_t CurrentCollectionIndex();
		uint32_t GetCollectionSize();
		UUID ID(const std::string& name);
		bool BOOL(const std::string& name);
		int INT(const std::string& name);
		float FLOAT(const std::string& name);
		std::vector<unsigned int> UINT_VECTOR(const std::string& name);
		std::vector<int> INT_VECTOR(const std::string& name);
		std::vector<uint32_t> UINT32_VECTOR(const std::string& name);
		std::vector<unsigned char> UCHAR_VECTOR(const std::string& name);
		std::vector<float> FLOAT_VECTOR(const std::string& name);
		std::vector<glm::vec2> VEC2_VECTOR(const std::string& name);
		std::vector<std::string> STRINGS_VECTOR(const std::string& name);
		std::vector<glm::vec3> VEC3_VECTOR(const std::string& name);
		std::vector<glm::vec4> VEC4_VECTOR(const std::string& name);
		std::vector<Fracture::SubMesh> SUBMESH_VECTOR(const std::string& name);
		std::string STRING(const std::string& name);
		glm::vec2 VEC2(const std::string& name);
		glm::vec3 VEC3(const std::string& name);
		glm::vec4 VEC4(const std::string& name);
		glm::quat QUAT(const std::string& name);
		ShaderRegistry SHADERREG(const std::string& name);
		SceneRegistry SCENEREG(const std::string& name);
		AnimationClipRegistry ANIMATIONREG(const std::string& name);


		json GetOutput();

		private:

		std::ofstream m_OutputStream;
		std::ifstream m_InputStream;

		std::stack<json> mStructStack;
		std::stack<std::string> mStructNameStack;

		std::stack<json::array_t> mCollectionStack;
		std::stack<std::string> mCollectionNameStack;
		std::stack<unsigned int> mCollectionIndex;

		json Output;
		IOMode mMode;
		SerialiseFormat mFormat;

		bool exists(const json& j, const std::string& key)
		{
			return j.find(key) != j.end();
		}

	};



}

#endif // !SERIALISER_H

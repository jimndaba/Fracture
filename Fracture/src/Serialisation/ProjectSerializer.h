#pragma once
#ifndef PROJECTSERIALIZER_H
#define PROJECTSERIALIZER_H


#include <fstream>
#include <memory>
#include <string>

#include "nlohmann/json.hpp"

namespace Fracture
{
	using json = nlohmann::json;
	class Scene;
	class Entity;
	struct ProjectProperties;

	class ProjectSerializer
	{
	public:
		ProjectSerializer(std::shared_ptr<ProjectProperties> properties);
		~ProjectSerializer();

		void Serialize(const std::string& filepath);
		bool DeSerializeProperties(const std::string& filepath);
		bool DeSerialize(const std::string& filepath);

		//void DeSerializeProperties(nlohmann::json entity);
		void DeSerializeModels(nlohmann::json entity);
		void DeSerializeShaders(nlohmann::json entity);
		void DeSerializeMaterial(nlohmann::json entity);
		void DeSerializeTextures(nlohmann::json entity);
		void DeSerializeHDRTextures(nlohmann::json entity);
		void DeSerializeScene(nlohmann::json entity);
		

		bool exists(const json& j, const std::string& key)
		{
			return j.find(key) != j.end();
		}

	private:
		std::ofstream m_OutputStream;	
		std::shared_ptr<ProjectProperties> m_properties;


	};

}

#endif
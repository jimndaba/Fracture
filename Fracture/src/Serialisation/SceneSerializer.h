#pragma once
#ifndef SCENESERIALIZER_H
#define SCENESERIALIZER_H

#include <fstream>
#include <memory>
#include <string>

#include "nlohmann/json.hpp"

namespace Fracture
{
	using json = nlohmann::json;
	class Scene;
	class Entity;

	class SceneSerializer
	{

	public:
		SceneSerializer(std::shared_ptr<Scene> scene);
		~SceneSerializer();

		void Serialize(const std::string& filepath);
		bool DeSerialize(const std::string& filepath);

		void SerializeComponents(nlohmann::json j);
		void DeSerializeEntity(nlohmann::json entity);

		bool exists(const json& j, const std::string& key)
		{
			return j.find(key) != j.end();
		}

	private:
		std::ofstream m_OutputStream;
		std::shared_ptr<Scene> m_scene;

	};

}

#endif
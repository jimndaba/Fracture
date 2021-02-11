#pragma once
#include <fstream>
#include <memory>
#include <string>

#include "nlohmann/json.hpp"

namespace Fracture
{
	using json = nlohmann::json;
	class Scene;
	class Entity;
	struct GameSettings;

	class GameSerializer
	{
	public:
		GameSerializer(std::shared_ptr<GameSettings> properties);
		~GameSerializer();

		void Serialize(const std::string& filepath);
		bool DeSerialize(const std::string& filepath);

		//void DeSerializeProperties(nlohmann::json entity);
		//void DeSerializeModels(nlohmann::json entity);
		//void DeSerializeShaders(nlohmann::json entity);
		//void DeSerializeMaterial(nlohmann::json entity);
		//void DeSerializeTextures(nlohmann::json entity);
		//void DeSerializeScene(nlohmann::json entity);


		bool exists(const json& j, const std::string& key)
		{
			return j.find(key) != j.end();
		}

	private:
		std::ofstream m_OutputStream;
		std::shared_ptr<GameSettings> m_settings;


	};

}

#include "GameSerializer.h"
#include "GameSettings.h"

Fracture::GameSerializer::GameSerializer(std::shared_ptr<GameSettings> gamesettings):m_settings(gamesettings)
{
}

Fracture::GameSerializer::~GameSerializer()
{
}

void Fracture::GameSerializer::Serialize(const std::string& filepath)
{
	m_OutputStream.open(filepath);
	json j;
	j["Title"] = m_settings->Title;
	j["IsFullScreen"] = m_settings->IsFullScreen;
	j["IsResizable"] = m_settings->IsResizable;
	j["Resolution_Width"] = m_settings->Resolution_Width;
	j["Resolution_Height"] = m_settings->Resolution_Height;
	m_OutputStream << j;
	m_OutputStream.close();
}

bool Fracture::GameSerializer::DeSerialize(const std::string& filepath)
{
	std::ifstream stream(filepath);
	json input;

	if (!stream.good())
	{
		//FRACTURE_ERROR("Can't read file");
		return false;
	}

	stream >> input;
	if (input.is_null())
	{
		//FRACTURE_ERROR("File is either non-json file or corrupt;");
		return false;
	}

	m_settings->Title = input["Title"];
	m_settings->IsFullScreen = input["IsFullScreen"];
	m_settings->IsResizable = input["IsResizable"];
	m_settings->Resolution_Height = input["Resolution_Height"];
	m_settings->Resolution_Width = input["Resolution_Width"];

	return false;
}

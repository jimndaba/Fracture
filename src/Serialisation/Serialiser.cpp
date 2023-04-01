#include "FracturePCH.h"
#include "Serialiser.h"
#include "Common/Logger.h"
#include "Rendering/Mesh.h"

Fracture::ISerialiser::ISerialiser(IOMode mode, SerialiseFormat format): mMode(mode),mFormat(format)
{
}

void Fracture::ISerialiser::Property(const std::string& name, uint32_t value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, int value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, bool value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, Fracture::UUID value)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = (uint32_t)value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back((uint32_t)value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const float& value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const char* value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const std::string& value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}	
}

void Fracture::ISerialiser::Property(const std::string& name, const Fracture::MeshRegistry& value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const Fracture::ShaderRegistry& value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const Fracture::SceneRegistry& value)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const Fracture::TextureRegistry& value)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const Fracture::MaterialRegistry& value)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const Fracture::LuaScriptRegistry& value)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const glm::vec2& value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const glm::vec3& value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const glm::vec4& value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const std::vector<unsigned int>& value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const std::vector<unsigned char>& value)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const std::vector<float>& value)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const std::vector<glm::vec2>& value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const std::vector<glm::vec3>& value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const std::vector<glm::vec4>& value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Property(const std::string& name, const std::vector<Fracture::SubMesh>& value)
{

	if (mStructStack.size() > mCollectionStack.size())
	{
		auto& j = mStructStack.top();
		j[name] = value;
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto& j = mCollectionStack.top();
		j.push_back(value);
	}
}

void Fracture::ISerialiser::Save(const std::string& path)
{
	if (!mStructStack.empty())
	{
		 FRACTURE_WARN("Current Struct stack not completed");
	}
	if (!mCollectionStack.empty())
	{
		FRACTURE_WARN("Current Struct stack not completed");
	}

	switch (mFormat)
	{
	case SerialiseFormat::Json:
	{
		m_OutputStream.open(path);
		m_OutputStream << Output;
		m_OutputStream.close();
		break;
	}
	case SerialiseFormat::Binary:
	{
		m_OutputStream.open(path, std::ios::out | std::ios::binary);
		// serialize it to BSON
		std::vector<std::uint8_t> v = json::to_bson(Output);
		m_OutputStream.write((const char*)v.data(), v.size() * sizeof(v[0]));
		m_OutputStream.close();
		break;
	}
	}

	{
		while (!mStructStack.empty())
			mStructStack.pop();

		while (!mStructNameStack.empty())
			mStructNameStack.pop();

		while (!mCollectionStack.empty())
			mCollectionStack.pop();

		while (!mCollectionNameStack.empty())
			mCollectionNameStack.pop();
	}
}

void Fracture::ISerialiser::Open(const std::string& path)
{
	switch (mFormat)
	{
	case SerialiseFormat::Json:
	{
		m_InputStream = std::ifstream(path);
		if (!m_InputStream.good())
		{
			FRACTURE_WARN("Can't read file : {}", path);
		}

		m_InputStream >> Output;
		if (Output.is_null())
		{
			FRACTURE_WARN("File is either non-json file or corrupt;");
		}
		FRACTURE_TRACE("Opening File: {}", path);
		break;
	}
	case SerialiseFormat::Binary:
	{
		m_InputStream = std::ifstream(path, std::ios::in | std::ios::binary);
		if (!m_InputStream.good())
		{
			FRACTURE_WARN("Can't read Scene file : {}", path);
		}

		// copies all data into buffer
		std::vector<std::uint8_t> buffer((std::istreambuf_iterator<char>(m_InputStream)), (std::istreambuf_iterator<char>()));
		m_InputStream.close();
		Output = json::from_bson(buffer); //this is where it is blowing up	
		break;
	}
	}
}

bool Fracture::ISerialiser::BeginStruct(const std::string& name)
{
	switch (mMode)
	{
	case IOMode::Save:
	{
		mStructStack.push(json());
		mStructNameStack.push(name);
		return true;
		break;
	}
	case IOMode::Open:
	{
		if (!Output.is_null())
		{
			if (!mStructStack.empty() && mCollectionStack.empty())
			{
				if (exists(mStructStack.top()[name], name))
				{
					auto j = mStructStack.top()[name];
					mStructStack.push(j);
					mStructNameStack.push(name);
					return true;
				}
				return false;
			}
			else if (!mCollectionStack.empty())
			{
				auto currentCollection = mCollectionStack.top();
				auto collection_name = mCollectionNameStack.top();
				if (exists(currentCollection[CurrentCollectionIndex()], name))
				{
					auto e = currentCollection[CurrentCollectionIndex()][name];
					if (!e.is_null())
					{
						mStructNameStack.push(name);
						mStructStack.push(e);
						return true;
					}
				}
				return false;
			}
			else
			{
				if (exists(Output, name))
				{
					auto j = Output[name];
					mStructStack.push(j);
					mStructNameStack.push(name);
					return true;
				}
			}
		}
	}
	}
	return false;
}

void Fracture::ISerialiser::EndStruct()
{
	switch (mMode)
	{
	case IOMode::Save:
	{
		if (!mCollectionNameStack.empty() && !mStructStack.empty())
		{
			auto& current_collection = mCollectionStack.top();
			auto& collection_name = mCollectionNameStack.top();

			auto& current_struct = mStructStack.top();
			auto& current_structname = mStructNameStack.top();

			json j;
			j[current_structname] = current_struct;
			current_collection.push_back(j);

			mStructStack.pop();
			mStructNameStack.pop();
		}
		else if (!mStructStack.empty() && mCollectionNameStack.empty())
		{
			auto current_struct = mStructStack.top();
			auto structname = mStructNameStack.top();
			mStructStack.pop();
			mStructNameStack.pop();


			if (!mStructStack.empty())
			{
				auto& next_struct = mStructStack.top();
				next_struct[structname] = current_struct;
			}
			else
			{
				Output[structname] = current_struct;
			}
		}
		break;
	}
	case IOMode::Open:
	{
		mStructStack.pop();
		mStructNameStack.pop();
		break;
	}
	}
}

bool Fracture::ISerialiser::BeginCollection(const std::string& name)
{
	switch (mMode)
	{
	case IOMode::Save:
	{
		mCollectionStack.push(json::array_t());
		mCollectionNameStack.push(name);
		return true;
		break;
	}
	case IOMode::Open:
	{
		if (!mStructStack.empty())
		{
			auto j = mStructStack.top();
			
			if (!exists(j, name))
				return false;

			mCollectionStack.push(j[name]);
			mCollectionNameStack.push(name);
			mCollectionIndex.push(0);
			return true;
		}
		if (!Output.is_null())
		{
			if (!exists(Output, name))
				return false;

			auto j = Output[name];
			mCollectionStack.push(j);
			mCollectionNameStack.push(name);
			mCollectionIndex.push(0);
			return true;
		}
		break;
	}
	}
	return false;
}

void Fracture::ISerialiser::NextInCollection()
{
	mCollectionIndex.top()++;
}

void Fracture::ISerialiser::EndCollection()
{
	switch (mMode)
	{
	case IOMode::Save:
	{
		if (mStructStack.empty() && !mCollectionStack.empty())
		{
			auto& current_collection = mCollectionStack.top();
			auto& current_collectionName = mCollectionNameStack.top();

			Output[current_collectionName] = current_collection;

			mCollectionStack.pop();
			mCollectionNameStack.pop();

		}
		if (!mStructStack.empty())
		{
			auto& current_struct = mStructStack.top();
			auto& current_structName = mStructNameStack.top();
			auto& collection_name = mCollectionNameStack.top();
			auto& current_collection = mCollectionStack.top();
			current_struct[collection_name] = current_collection;

			mCollectionNameStack.pop();
			mCollectionStack.pop();
		}
		break;
	}
	case IOMode::Open:
	{
		mCollectionNameStack.pop();
		mCollectionStack.pop();
		mCollectionIndex.pop();
		break;
	}
	}
}

uint32_t Fracture::ISerialiser::CurrentCollectionIndex()
{
	return mCollectionIndex.top();
}

uint32_t Fracture::ISerialiser::GetCollectionSize()
{
	return (uint32_t)mCollectionStack.top().size();
}

Fracture::UUID Fracture::ISerialiser::ID(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return (uint32_t)j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		auto uid = (uint32_t)j.at(mCollectionIndex.top())[name];
		return uid;
	}	
	return UUID();
}

bool Fracture::ISerialiser::BOOL(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}

	return 0;
}

int Fracture::ISerialiser::INT(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	/*
	if (!mCollectionStack.empty())
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	else if (!mStructStack.empty() && mCollectionStack.empty())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	*/
	return 0;
}

float Fracture::ISerialiser::FLOAT(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	/*
	if (!mCollectionStack.empty())
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	else if (!mStructStack.empty() && mCollectionStack.empty())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
		{
			return j[name];
		}
	}
	*/
	return 0.0f;
}

std::vector<unsigned int> Fracture::ISerialiser::UINT_VECTOR(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	return std::vector<unsigned int>();
}

std::vector<unsigned char> Fracture::ISerialiser::UCHAR_VECTOR(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	return std::vector<unsigned char>();
}

std::vector<float> Fracture::ISerialiser::FLOAT_VECTOR(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	return std::vector<float>();
}

std::vector<glm::vec2> Fracture::ISerialiser::VEC2_VECTOR(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	/*
	if (!mCollectionStack.empty())
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	else if (!mStructStack.empty() && mCollectionStack.empty())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	*/
	return std::vector<glm::vec2>();
}

std::vector<glm::vec3> Fracture::ISerialiser::VEC3_VECTOR(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	/*
	if (!mCollectionStack.empty())
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	else if (!mStructStack.empty() && mCollectionStack.empty())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	*/
	return std::vector<glm::vec3>();
}

std::vector<glm::vec4> Fracture::ISerialiser::VEC4_VECTOR(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	/*
	if (!mCollectionStack.empty())
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	else if (!mStructStack.empty() && mCollectionStack.empty())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	*/
	return std::vector<glm::vec4>();
}

std::vector<Fracture::SubMesh> Fracture::ISerialiser::SUBMESH_VECTOR(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	/*
	if (!mCollectionStack.empty())
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	else if (!mStructStack.empty() && mCollectionStack.empty())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	*/
	return std::vector<Fracture::SubMesh>();
}

std::string Fracture::ISerialiser::STRING(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	/*
	if (!mCollectionStack.empty())
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	else if (!mStructStack.empty() && mCollectionStack.empty())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			
			*/
	return "";
}

glm::vec2 Fracture::ISerialiser::VEC2(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	/*
	if (!mCollectionStack.empty())
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	else if (!mStructStack.empty() && mCollectionStack.empty())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
		{
			return j[name];
		}
	}
	*/
	return glm::vec2();
}

glm::vec3 Fracture::ISerialiser::VEC3(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	/*
	if (!mCollectionStack.empty())
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	else if (!mStructStack.empty() && mCollectionStack.empty())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	*/
	return glm::vec3(0.0);
}

glm::vec4 Fracture::ISerialiser::VEC4(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	/*
	if (!mCollectionStack.empty())
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	else if (!mStructStack.empty() && mCollectionStack.empty())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	*/
	return glm::vec4();
}

Fracture::ShaderRegistry Fracture::ISerialiser::SHADERREG(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	/*
	if (!mCollectionStack.empty())
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	else if (!mStructStack.empty() && mCollectionStack.empty())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	*/
	return ShaderRegistry();
}

Fracture::SceneRegistry Fracture::ISerialiser::SCENEREG(const std::string& name)
{
	if (mStructStack.size() > mCollectionStack.size())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	else if (!mCollectionStack.empty() && (mCollectionStack.size() <= mStructStack.size()))
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	/*
	if (!mCollectionStack.empty())
	{
		auto j = mCollectionStack.top();
		return j.at(mCollectionIndex.top())[name];
	}
	else if (!mStructStack.empty() && mCollectionStack.empty())
	{
		auto j = mStructStack.top();
		if (exists(j, name))
			return j[name];
	}
	*/
	return SceneRegistry();
}

nlohmann::json Fracture::ISerialiser::GetOutput()
{
	return Output;
}

#pragma once
#ifndef FRAMEGRAPHSERIALISER_H
#define FRAMEGRAPHSERIALISER_H

#include <memory>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

namespace Fracture
{
	class FrameGraph;
	using json = nlohmann::json;

	class FramaGraphSerialiser
	{
	public:
		FramaGraphSerialiser(const std::shared_ptr<FrameGraph>&  graph);
		void SerialiseGraph(const std::string& filepath);
		bool DeSerialiseGraph(const std::string& filepath);
	private:
		std::shared_ptr<FrameGraph>  m_graph;
		std::ofstream m_OutputStream;

		void DeserializeNode(json node);
		void DeserializeLink(json link);

		bool exists(const json& j, const std::string& key)
		{
			return j.find(key) != j.end();
		}
	};


}

#endif
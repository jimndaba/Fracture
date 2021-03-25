#pragma once
#ifndef FRAMEGRAPHSERIALISER_H
#define FRAMEGRAPHSERIALISER_H

#include <memory>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "Rendering/Renderer.h"
#include "Rendering/Framegraph/FrameGraph.h"

namespace Fracture
{
	using json = nlohmann::json;

	class FrameGraphSerialiser
	{
	public:
		FrameGraphSerialiser(std::shared_ptr<FrameGraph>&  graph, Renderer& renderer);
		void SerialiseGraph(const std::string& filepath);
		bool DeSerialiseGraph(const std::string& filepath);
	private:
		std::shared_ptr<FrameGraph>&  m_graph;
		std::ofstream m_OutputStream;
		Renderer& m_renderer;

		void DeserializeNode(json node);
		void DeserializeLink(json link);

		bool exists(const json& j, const std::string& key)
		{
			return j.find(key) != j.end();
		}
	};


}

#endif
#pragma once
#ifndef FRAMEGRAPHSERIALISER_H
#define FRAMEGRAPHSERIALISER_H

#include <memory>
#include <string>
#include <fstream>

namespace Fracture
{
	class FrameGraph;

	class FramaGraphSerialiser
	{
	public:
		FramaGraphSerialiser(const std::shared_ptr<FrameGraph>&  graph);
		void SerialiseGraph(const std::string& filepath);
		void DeSerialiseGraph();
	private:
		std::shared_ptr<FrameGraph>  m_graph;
		std::ofstream m_OutputStream;
	};


}

#endif
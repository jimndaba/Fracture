#pragma once
#ifndef FRAMENODE_H
#define FRAMENODE_H

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "InputSocket.h"
#include "OutputSocket.h"
#include "FrameResource.h"
#include "Logging/Logger.h"
#include "Serialisation/FrameGraphSerialisation/FrameNodeSerialiser.h"

namespace Fracture
{
	class Renderer;


	class FrameNode
	{
	public:
		FrameNode(const std::string& name);

		virtual void execute(Renderer& renderer) = 0;

		void AddInputSocket(std::shared_ptr<InputSocket> input);
		void AddOutputSocket(std::shared_ptr<OutputSocket> output);

		void AddInputResource(std::shared_ptr<InputSocket> input, std::shared_ptr<FrameResource> resource);
		void AddOutputResource(std::shared_ptr<OutputSocket> output, std::shared_ptr<FrameResource> resource);

		void AddResource(std::string name, std::shared_ptr<FrameResource> res);
		std::shared_ptr<FrameResource> getResource(std::string name);

		void LinkResource(std::string name, std::shared_ptr<FrameResource> res);

		std::string GetName() const;
		void Resize(unsigned int width, unsigned int height);

		virtual json Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor) = 0;

		bool isVisited;
		std::map<std::string, std::shared_ptr<FrameResource>> resources;
	private:
		std::string m_name;
		std::vector<std::shared_ptr<InputSocket>> m_inputs;
		std::vector<std::shared_ptr<OutputSocket>> m_outputs;
	
	};


}

#endif
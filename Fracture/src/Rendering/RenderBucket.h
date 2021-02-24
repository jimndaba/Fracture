#pragma once
#ifndef RENDERBUCKET_H
#define RENDERBUCKET_H

#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include <map>

#include "DrawCommand.h"

namespace Fracture
{

	class RenderComponent;
	class TransformComponent;
	class Mesh;
	class Material;
	class RenderBatch;

	class RenderBucket
	{
	public:
		RenderBucket();
		~RenderBucket();
		void pushCommand(const DrawCommand& command);
		void pushCommand(const UUID& EntityID, glm::vec4 color, std::shared_ptr<Fracture::Mesh> mesh, std::shared_ptr<Fracture::Material> material, glm::mat4 transform);
		void pushOutlineCommand(const UUID& EntityID, std::shared_ptr<Fracture::Mesh> mesh, glm::mat4 transform);
		void sortForward();
		void sortAlpha();
		void clear();

		std::vector<DrawCommand> getForwardRenderCommands();
		std::vector<DrawCommand> getAlphaRenderCommands();
		std::vector<DrawCommand> getShadowRenderCommands();
		std::vector<DrawCommand> getOutlineRenderCommands();
	private:
		std::vector<DrawCommand> m_Frowardcommands;
		std::vector<DrawCommand> m_Alphacommands;
		std::vector<DrawCommand> m_Shadowcommands;
		std::vector<DrawCommand> m_Outlinecommands;
	};

}

#endif
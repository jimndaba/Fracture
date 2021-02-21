#pragma once
#ifndef POINTSHADOWS_H
#define POINTSHADOWS_H

#include "../RenderQueueNode.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Fracture
{
	class PointLight;

	class PointShadowsNode : public RenderQueueNode
	{
	public:
		PointShadowsNode(const std::string& name, const int& width, const int& height, std::shared_ptr<RenderBucket> bucket, const std::shared_ptr<PointLight>& light);
		void execute(Renderer& renderer) override;

		void PrepareLightMatrix();

	private:
		int Width;
		int Height;
		float aspect;
		float m_near = 1.0f;
		float m_far = 25.0f;
		glm::mat4 shadowProj;
		std::shared_ptr<FrameResource> outShadowMap;
		std::vector<glm::mat4> shadowTransforms;
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<PointLight> m_light;
	};
}

#endif
#pragma once
#ifndef SHADOWPASS_H
#define SHADOWPASS_H

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

namespace Fracture
{
	class SunLight;
	class Material;
	class RenderTarget;
	class RenderCommand;
	class RenderBucket;

	class ShadowPass
	{

	public:
		ShadowPass( );
		~ShadowPass();
		void Begin();
		void End();
		void Prepare(std::shared_ptr<SunLight> light);
		void Render(std::shared_ptr<Material> material, RenderBucket& command);
		void Bind();
		void unBind();

		glm::mat4 GetLightSpaceMatrix();
		void SetOrthor(float l, float r, float t, float b);
		void SetNearFarPlanes(float n, float f);
		glm::vec2 GetNearFarPlanes();
		glm::vec4 GetOrthor();

		std::shared_ptr<RenderTarget> GetRenderTarget();

	private:
		const unsigned int SHADOW_WIDTH = 1024;
		const unsigned int SHADOW_HEIGHT = 1024;
		float near_plane = -10.00f;
		float far_plane = 100.0f;
		float m_left = -25.0f;
		float m_right = 25.0f;
		float m_bottom = -25.0f;
		float m_top = 25.0f;
		glm::mat4 m_lightspaceMatrix;

		std::shared_ptr<RenderTarget> m_renderTarget;
	};


}
#endif
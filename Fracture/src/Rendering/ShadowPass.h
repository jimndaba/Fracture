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

	class ShadowPass
	{

	public:
		ShadowPass( );
		~ShadowPass();
		void Begin();
		void End();
		void Prepare(std::shared_ptr<SunLight> light);
		void Render();
		void Bind();

		glm::mat4 GetLightSpaceMatrix();
		void SetOrthor(float l, float r, float t, float b);
		void SetNearFarPlanes(float n, float f);

		std::shared_ptr<RenderTarget> GetRenderTarget();

	private:
		const unsigned int SHADOW_WIDTH = 1024;
		const unsigned int SHADOW_HEIGHT = 1024;
		float near_plane = 0.1f;
		float far_plane = 20.0f;
		float m_left = -20.0f;
		float m_right = 20.0f;
		float m_bottom = -20.0f;
		float m_top = 20.0f;
		glm::mat4 m_lightspaceMatrix;

		std::shared_ptr<RenderTarget> m_renderTarget;
	};


}
#endif
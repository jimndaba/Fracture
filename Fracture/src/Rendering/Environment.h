#pragma once
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "GLAD/glad.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>


namespace Fracture
{

	class RenderTarget;
	class FrameBuffer;
	class Texture2D;
	class TextureCubeMap;
	class Model;
	class Shader;

	class Environment
	{

	public: 
		Environment(std::shared_ptr<Texture2D> environment, std::shared_ptr<Shader> shader);
		~Environment();

		//unsigned int envCubemap;
		//unsigned int irradianceMap;
		//unsigned int prefilterMap;
		//unsigned int brdfLUTTexture;

		std::shared_ptr<RenderTarget> m_CaptureTarget;
	
		std::shared_ptr<Shader> m_irradiance;
		std::shared_ptr<Shader> m_prefilter;
		std::shared_ptr<Shader> m_bdrf;

		std::shared_ptr<Texture2D> m_enviroment;
		std::shared_ptr<Texture2D> m_bdrfTexture;

		std::shared_ptr<TextureCubeMap> m_PrefilterMap;
		std::shared_ptr<TextureCubeMap> m_IrradianceMap;
		std::shared_ptr<TextureCubeMap> m_CubeMap;
		
		void Render(std::shared_ptr<Shader> shader, glm::mat4 view, glm::mat4 projection);
	private:
		unsigned int cubeVAO;
		unsigned int cubeVBO;
		unsigned int quadVAO = 0;
		unsigned int quadVBO;
		
		std::shared_ptr<Shader> shader;

		glm::mat4 captureProjection;
		glm::mat4 captureViews[6];

		void CreateCubeMaptexture();
		void SetupMatricies();
		void ConvertHDRtoCubeMap();
		void CreateIrradianceMap();
		void CreatePreFilterMap();
		void CreateBDRF();

		void Render();
		void RenderQuad();
	};
}

#endif
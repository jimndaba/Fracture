#pragma once
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <string>
#include "AssetManager/AssetManager.h"

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
		Environment(const std::string& name,const std::shared_ptr<Shader>& rendershader);

		~Environment();

		
		std::shared_ptr<Shader> m_irradiance;
		std::shared_ptr<Shader> m_prefilter;
		std::shared_ptr<Shader> m_bdrf;
		std::shared_ptr<Texture2D> m_bdrfTexture;
		std::shared_ptr<TextureCubeMap> m_PrefilterMap;
		std::shared_ptr<TextureCubeMap> m_IrradianceMap;

		std::shared_ptr<RenderTarget> m_CaptureTarget;
		std::shared_ptr<Texture2D> m_enviroment;
		
		
		std::shared_ptr<TextureCubeMap> m_CubeMap;
		
		void Render(glm::mat4 view, glm::mat4 projection);

		void RenderCube();
		void RenderQuad();

		void CreateCubeMaptexture();
		void SetupMatricies();
		void SetupDynameCubemap();
		void ConvertHDRtoCubeMap();
		void CreateIrradianceMap();
		void CreatePreFilterMap();
		void CreateBDRF();

		std::shared_ptr<Shader> GetShader();

	private:
		unsigned int cubeVAO = 0;
		unsigned int cubeVBO = 0;
		unsigned int quadVAO = 0;
		unsigned int quadVBO = 0;
		
		std::shared_ptr<Shader> m_Cubeshader;
		std::shared_ptr<Shader> m_Rendershader;

		glm::mat4 captureProjection;
		glm::mat4 captureViews[6];
		
	};
}

#endif
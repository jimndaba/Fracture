#pragma once
#ifndef SHADOWCASTER_H
#define SHADOWCASTER_H

#include "IPass.h"

namespace Fracture
{
	struct VertexArray;
	struct CameraComponent;

	struct ShadowCasterPassDef
	{
		int CascadeCont;
		float Bias = 0.01f;
		int SpotResolution = 512;
		int PointResolution = 256;
	};


	struct ShadowCasterPass : public IPass
	{
		ShadowCasterPass(const std::string& name, RenderContext* context, const ShadowCasterPassDef& info);

		void Setup();
		void Execute();

		ShadowCasterPassDef Properties;

		bool IsDirty = true;
	private:
		uint32_t Vao;
		std::vector<float> shadowCascadeLevels;

		std::shared_ptr<Buffer> mMatrixBuffer;
		std::shared_ptr<Buffer> mSplaneDistances;	

		std::shared_ptr<Shader> mShader;
		std::shared_ptr<Shader> mShaderInstanced;
		std::shared_ptr<Shader> mSpotShader;
		std::shared_ptr<Shader> mPointShader;
		std::shared_ptr<Shader> mShaderSkinned;
		//Create properties to adjust cascade levels


		std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view);
		glm::mat4 getLightSpaceMatrix(const float fov, const float Cnear, const float Cfar, const glm::mat4 view, glm::vec3 cam_pos, glm::vec3 cam_forward);
		std::vector<glm::mat4> getLightSpaceMatrices(CameraComponent* camera);
	};

}

#endif
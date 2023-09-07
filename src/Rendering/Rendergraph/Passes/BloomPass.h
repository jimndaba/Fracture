#pragma once
#ifndef BLOOMPASS_H
#define BLOOMPASS_H

#include "IPass.h"
#include "IPostProcess.h"


namespace Fracture
{

	struct BloomPassDef
	{
	};

	struct BloomMipMap
	{
		glm::vec2 size;
		glm::ivec2 isize;
		std::shared_ptr<Texture>  texture;
	};


	struct BloomPass : public IPostProcess
	{
		BloomPass();

		void Setup();
		bool Render(PostProcessInfo info);

		BloomPassDef Properties;

		std::shared_ptr<Shader> bloomfinal;
		std::shared_ptr<Shader> upsampleShader;
		std::shared_ptr<Shader> downsampleShader;
		std::shared_ptr<Shader> brightpassShader;
		std::shared_ptr<Shader> BloomPrefilterShader;


		std::shared_ptr<Texture> mDirtMask;
		std::shared_ptr<Texture> mPrefilteredTexture;

		std::shared_ptr<RenderTarget> mBloomFBO;
		std::vector<std::shared_ptr<BloomMipMap>>  BloomMipMaps;
	};





}

#endif
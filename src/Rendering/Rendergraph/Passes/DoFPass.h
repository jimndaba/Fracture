#pragma once
#ifndef DOFPASS_H
#define DOFPASS_H


#include "IPostProcess.h"

namespace Fracture
{
	struct Texture;
	struct RenderTarget;

	struct DoFPass : public IPostProcess
	{
		void Setup();
		bool Render(PostProcessInfo info);
		std::shared_ptr<Shader> CoCshader;
		std::shared_ptr<Shader> CoCDownSample;
		std::shared_ptr<Shader> BokehShader;
		std::shared_ptr<Shader> BokehBlurShader;
		std::shared_ptr<Shader> DoFCombine;
		std::shared_ptr<Texture> mCoCTexture;
		std::shared_ptr<Texture> mCoCDownSampledTexture;
		std::shared_ptr<Texture> mBokehTexture;
		std::shared_ptr<Texture> mBokehBlurTexture;

		std::shared_ptr<RenderTarget> mDoFTarget;

	};









}


#endif
#pragma once
#ifndef POSTPROCESSINGPARAMS_H
#define POSTPROCESSINGPARAMS_H


namespace Fracture
{
	enum class ToneMappingMode : int
	{
		HDRTone2,
		Uncharted,
		Uncharted2,
		Filmic,
		AcesFilm,
		Reinhard
	};

	struct GlobalPostProcessParams
	{
		//SSAO Parmas
		bool SSAO_Enabled = 1;
		float SSAO_Base = 0.2f;
		float SSAO_Bias = 0.025f;
		float SSAO_Radius = 0.5f;
		float SSAO_Falloff = 0.00001f;
		float SSAO_Area = 0.0001f;
		int SSAO_BlurSize = 10;
		int SSAO_NoiseSize = 16;
		int SSAO_NoSamples = 4;
		float SSAO_Strength = 1.0f;
		glm::vec2 SSAO_Resolution = { 1920,1080};
		
		//Cascade Shadows
		int ShadowCascadeCont;
		float ShadowBias = 0.01f;
		int Shadow_Resolution = 2048;

		//FXAA
		bool FXAA_Enabled = 1;
		float FXAA_MaxSpan = 8.0f;
		float FXAA_MidReduce = 128.0f;
		float FXAA_SHIFT = 4.0f;

		//ToneMapping	
		bool ToneMapping_Enabled = 1;
		float ToneMapping_Exposure = 1.0f;
		float ToneMapping_Gamma = 2.2f;
		float ToneMapping_Contrast = 1.0f;
		float ToneMapping_Brightness = 0.0f;
		int ToneMapping_ToneMapMode = 0;
		glm::vec3 pad;
	};





}

#endif
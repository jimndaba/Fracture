#pragma once
#ifndef POSTPROCESSINGPARAMS_H
#define POSTPROCESSINGPARAMS_H


namespace Fracture
{

	struct GlobalPostProcessParams
	{
		//SSAO Parmas
		float SSAO_Base = 0.2f;
		float SSAO_Radius = 0.5f;
		float SSAO_Falloff = 0.00001f;
		float SSAO_Area = 0.075f;
		int SSAO_NoSamples = 18;
		float SSAO_Strength = 1.0f;
		glm::vec2 SSAO_Resolution = { 1280,720 };
		//

	};





}

#endif
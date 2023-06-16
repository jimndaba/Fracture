#pragma once
#ifndef PPGLOBALS_H
#define PPGLOBALS_H

namespace Fracture
{

	struct PostProcessingGlobals
	{
		//SSR Globals
		float SSR_Step = 0.1f;
		float SSR_MinRayStep = 0.1f;
		float SSR_MaxSteps = 30.0f;
		int SSR_NumBinarySearchSteps = 5;
		float SSR_ReflectionSpecularFalloffExponent = 3.0f;
		float SSR_Strength = 0.1f;





	};

}

#endif 


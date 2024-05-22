#pragma once
#ifndef WINDSYSTEM_H
#define WINDSYTEM_H


namespace Fracture
{

	struct WindSystemData
	{
		float WindStrength = 0.1;
		float WindSpeed = 2.0;
		//Perlin Noise Params
		float frequency = 1.0;
		float amplitude = 1.0;

		glm::vec4 WindDirection = glm::vec4(1.0, 0.0, 0.0,0.0); // Wind blowing in the x-direction	
	};





}




#endif
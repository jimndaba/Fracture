#pragma once
#ifndef GPUDATA_H
#define GPUDATA_H

namespace Fracture
{
	struct VolumeTileAABB
	{
		glm::vec4 min;
		glm::vec4 max;
	};


	struct ScreenToView {
		unsigned int tileSizes[4];
		unsigned int screenWidth;
		unsigned int screenHeight;
		float sliceScalingFactor;
		float sliceBiasFactor;
	};


}

#endif
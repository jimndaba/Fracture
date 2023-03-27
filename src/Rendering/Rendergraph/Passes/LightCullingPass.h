#pragma once
#ifndef LIGHTCULLPASS_H
#define LIGHTCULLPASS_H

#include "IPass.h"
#include "Rendering/GPUData.h"


namespace Fracture
{
	struct LightCullPassDef
	{
		const unsigned int gridSizeX = 16;
		const unsigned int gridSizeY = 9;
		const unsigned int gridSizeZ = 24;
		const unsigned int numClusters = gridSizeX * gridSizeY * gridSizeZ;
		unsigned int sizeX, sizeY;
		unsigned int numLights;
		const unsigned int maxLights = 1000; // pretty overkill for sponza, but ok for testing
		const unsigned int maxLightsPerTile = 50;

		ScreenToView screen2View;
	};


	struct LightCullPass : public IPass
	{
		LightCullPass(const std::string& name, RenderContext* context, const  LightCullPassDef& info);

		void Setup();
		void Execute();

		LightCullPassDef Properties;

		std::shared_ptr<Shader> mClusterComputeShader;
		std::shared_ptr<Shader> mLightCullComputeShader;

		std::shared_ptr<Buffer> VolumeGridSSBO;
		std::shared_ptr<Buffer> ScreenToViewSSBO;
		std::shared_ptr<Buffer> lightIndexListSSBO;
		std::shared_ptr<Buffer> lightGridSSBO;
		std::shared_ptr<Buffer> lightIndexGlobalCountSSBO;

		bool dirty = true;
	};



}

#endif
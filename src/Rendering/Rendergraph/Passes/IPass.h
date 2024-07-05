#pragma once
#ifndef IPASS_H
#define IPASS_H

#include <Rendering/Color.h>
#include "Rendering/RenderContext.h"
#include "Rendering/RenderCommands.h"
#include "Rendering/GraphicsDevice.h"
#include "World/SceneManager.h"
#include "Assets/AssetManager.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Animation/AnimationSystem.h"
#include "Rendering/PostProcessPipeline.h"
#include "World/LightProbeSystem.h"
#include "Rendering/DebugRenderer.h"


namespace Fracture
{
	struct RenderContext;

	struct Socket
	{
		std::string Name;
		std::string Resource;
		bool HasBeenVisited;
	};

	struct PassLink
	{
		std::string From;
		std::string From_Resource;

		std::string To;
		std::string To_Resource;
	};

	struct IPass
	{
		IPass(const std::string& name, RenderContext* context);
		~IPass() = default;
		virtual void Setup() = 0;
		virtual void Execute() = 0;

		Fracture::UUID ID;
		std::string Name;
		bool IsVisited = false;
		std::vector<std::string> Inputs;
		std::vector<std::string> Outputs;
		RenderContext* Context;
	};




}

#endif
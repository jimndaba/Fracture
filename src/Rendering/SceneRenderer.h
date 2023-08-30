#pragma once
#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include "Rendering/Rendergraph/Passes/ForwardPass.h"
#include "Rendering/Rendergraph/Passes/ZPrePass.h"
#include "Rendering/Rendergraph/Passes/LightCullingPass.h"
#include "Rendering/Rendergraph/Passes/ComposeRenderersPass.h"
#include "Rendering/Rendergraph/Passes/SSAOPass.h"
#include "Rendering/Rendergraph/Passes/ShadowCasterPass.h"
#include "Rendering/Rendergraph/Passes/SSRPass.h"
#include "Rendering/Rendergraph/Passes/PresentPass.h"
#include "Rendering/Rendergraph/Passes/GridPass.h"
#include "Rendering/Rendergraph/Passes/OutlinePass.h"

namespace Fracture
{

	class SceneRenderer
	{

	public:
		SceneRenderer();

		void Init();

		void Begin(float dt);



		void QueueLightProbesToBake(UUID id);


		void End();

		std::shared_ptr<Fracture::RenderContext> mContext;
		std::shared_ptr<Fracture::RenderContext> mFinalContext;
		std::shared_ptr<Fracture::ZPrePass> zPrePass;
		std::shared_ptr<Fracture::SSAOPass> ssaoPass;
		std::shared_ptr<Fracture::SSRPass> ssrPass;
		std::shared_ptr<Fracture::GridPass> gridpass;
		std::shared_ptr<Fracture::ShadowCasterPass> shadaowPass;
		std::shared_ptr<Fracture::ForwardPass> f_pass;
		std::shared_ptr<Fracture::OutlinePass> Outline_pass;
		std::shared_ptr<Fracture::LightCullPass> lightcullPass;
		std::shared_ptr<Fracture::ComposeRenderersPass> ComposePass;
		std::shared_ptr<Fracture::PresentPass> presentPass;
		std::queue<Fracture::UUID> mLightProbesToRender;

		bool DrawGrid = false;
		Fracture::GlobalFrameData data;
	};



}
#endif
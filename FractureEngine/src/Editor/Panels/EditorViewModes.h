#pragma once
#ifndef SSAOVIEW_H
#define SSAOVIEW_H

#include "ViewportMode.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"

#include "Fracture.h"

namespace Fracture
{
	class RenderViewMode :public ViewportMode
	{
	public:

		RenderViewMode() :ViewportMode()
		{

		}
		void Render(const std::shared_ptr<FrameGraph>& graph, const ImVec2& size) override
		{
			ImGui::Image((ImTextureID)(graph->GetOutput()->outputColor->GetColorTexture(0)->GetTextureID()),
				size, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		}
	};

	class SSAOViewMode :public ViewportMode 
	{
	public:

		SSAOViewMode() :ViewportMode()
		{

		}
		void Render(const std::shared_ptr<FrameGraph>& graph,const ImVec2& size) override
		{
			ImGui::Image((ImTextureID)(graph->getNode("ssaoPass")->resources["SSAOOutput"]->GetColorTexture(0)->GetTextureID()),
				size, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		}
	};

	class DepthViewMode :public ViewportMode
	{
	public:

		DepthViewMode() :ViewportMode()
		{

		}
		void Render(const std::shared_ptr<FrameGraph>& graph, const ImVec2& size) override
		{
			ImGui::Image((ImTextureID)(graph->getNode("global_depthbuffer")->resources["outputDepthMap"]->GetColorTexture(0)->GetTextureID()),
				size, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		}
	};


}

#endif
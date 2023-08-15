#pragma once
#ifndef POSTPROCESSPIPELINE_H
#define POSTPROCESSPIPELINE_H

namespace Fracture
{
	struct RenderTarget;
	struct RenderContext;
	struct IPostProcess;

	struct PostProcessPipeline
	{
		PostProcessPipeline();

		void Init();
		void OnSetupPipeline();
		void OnRender();
		void Submit();

		void SwapBuffers();
		uint32_t GetRenderTargetToWrite();
		uint32_t GetRenderTargetToReadFrom();
		uint32_t GetOutputImage();
		std::shared_ptr<RenderTarget> GetOutputRenderTarget();


		//Will Have Post Processing Passes.
		//Will also have PingPong Buffers
		std::vector<std::shared_ptr<RenderTarget>> mPingPongBuffer;
		std::unique_ptr<RenderContext> Context;
		std::vector<std::shared_ptr<IPostProcess>> mPostProcessStack;
		
		bool m_pingpong_read = false;
		uint32_t Vao;
	};




}

#endif
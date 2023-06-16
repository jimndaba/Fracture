#include "FracturePCH.h"
#include "PostProcessPipeline.h"
#include "GraphicsDevice.h"
#include "RenderTarget.h"
#include "RenderCommands.h"
#include "Rendering/Rendergraph/Passes/IPostProcess.h"
#include "Rendering/Rendergraph/Passes/FXAAPass.h"
#include "Rendering/Rendergraph/Passes/ToneMappingPass.h"

Fracture::PostProcessPipeline::PostProcessPipeline()
{
}


void Fracture::PostProcessPipeline::Init()
{
	//PingPong Buffers;
	RenderTargetCreationInfo desc;
	TextureCreationInfo info;
	info.AttachmentTrgt = AttachmentTarget::Color;
	info.Width = 1920;
	info.Height = 1080;
	info.formatType = TextureFormatType::Float;
	info.format = TextureFormat::RGBA;
	info.internalFormat = InternalFormat::RGBA16F;
	info.TextureTarget = TextureTarget::Texture2D;
	info.magFilter = TextureMagFilter::Linear;
	info.minFilter = TextureMinFilter::Linear;
	info.Wrap = TextureWrap::ClampToEdge;
	desc.ColorAttachments.push_back(info);
	desc.IsResizable = true;
	

	mPingPongBuffer.resize(2);

	for (int i = 0; i < 2; i++)
	{
		mPingPongBuffer[i] = GraphicsDevice::Instance()->CreateRenderTarget(desc);
		std::string name = "PingPongBuffer" + std::to_string(i);
		mPingPongBuffer[i]->Info.Name = name.c_str();
	}

	Context = std::make_unique<RenderContext>();

	VertexArrayCreationInfo vao_info;
	GraphicsDevice::Instance()->CreateVertexArray(Vao, vao_info);
}

void Fracture::PostProcessPipeline::OnSetupPipeline()
{

	/*
	* https://gamedev.stackexchange.com/questions/147952/what-is-the-order-of-postprocessing-effects
	Ambient occlusion
	Screenspace reflection
	Motion blur (input is HDR so bright reflections smear badly into solid bright streaks which doesn't look good)
	TAA (yes - for me it's weird to stabilize motion blurred, mostly noisy pixels)
	DoF
	Chromatic abberration
	Bloom
	Vignette
	Colorgrading (with tonemapping that brings values from HDR to LDR)
	Next steps operates on LDR values:

	User LUT
	Grain
	Dithering (if grain is inactive
	*/

	//HDR Post Processing
	mPostProcessStack.push_back(std::make_shared<FXAAPass>());




	// HDR to LDR
	mPostProcessStack.push_back(std::make_shared<ToneMappingPass>());

	//LDR Post Processing

	//Set up all passes
	for (const auto& step : mPostProcessStack)
	{
		step->Setup();
	}
}



void Fracture::PostProcessPipeline::OnRender()
{
	auto input_texture = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalColour)->ColorAttachments[0];
	const auto& cntxt = Context.get();

	Fracture::SortKey key;
	cntxt->BeginState(key);
	cntxt->Begin();

	for (const auto& target : mPingPongBuffer)
	{
		RenderCommands::SetRenderTarget(cntxt, target->Handle);
		RenderCommands::ClearTarget(cntxt, (uint32_t)Fracture::ClearFlags::Color);
		RenderCommands::ClearColor(cntxt,Fracture::Colour::Red);
		RenderCommands::SetViewport(cntxt, 1920, 1080, 0, 0);
		RenderCommands::SetScissor(cntxt, 1920, 1080, 0, 0);
	}

	RenderCommands::SetRenderTarget(cntxt,(uint32_t)0);
	
	//Entry Point
	PostProcessInfo info =
	{
		.cntxt = cntxt,
		.write_to = GetRenderTargetToWrite(),
		.input_texture = input_texture->Handle,
		.VAO = Vao
	};
	mPostProcessStack[0]->Render(info);
	SwapBuffers();

	//Do Rest of the stack
	for (int i = 1; i < mPostProcessStack.size(); i++)
	{
		
		PostProcessInfo minfo =
		{
			.cntxt = cntxt,
			.write_to = GetRenderTargetToWrite(),
			.input_texture = GetRenderTargetToReadFrom(),
			.VAO = Vao
		};
		mPostProcessStack[i]->Render(minfo);	
		SwapBuffers();
	}

	cntxt->EndState();
}

void Fracture::PostProcessPipeline::Submit()
{
	Context->Render();
}

void Fracture::PostProcessPipeline::SwapBuffers()
{
	m_pingpong_read = !m_pingpong_read;
}

uint32_t Fracture::PostProcessPipeline::GetRenderTargetToWrite()
{
	return mPingPongBuffer[(int)m_pingpong_read]->Handle;
}
uint32_t Fracture::PostProcessPipeline::GetRenderTargetToReadFrom()
{
	return mPingPongBuffer[!(int)m_pingpong_read]->ColorAttachments[0]->Handle;
}

uint32_t Fracture::PostProcessPipeline::GetOutputImage()
{
	return mPingPongBuffer[!(int)m_pingpong_read]->ColorAttachments[0]->Handle;
}

#include "pch.h"
#include "DepthOfFieldNode.h"
#include "core/Application.h"
#include "core/GameWindow.h"
#include "rendering/Device.h"
#include "rendering/Pipeline.h"
#include "assets/TextureLoader.h"
#include "core/Components.h"

Fracture::DepthOfFieldNode::DepthOfFieldNode(DeviceContext& context, const std::string& name, const std::vector<std::string>& inputs, const std::vector<std::string>& outputs)
	:RenderNode(context, name, inputs, outputs)
{
}

void Fracture::DepthOfFieldNode::Setup()
{
	{
		ShaderDescription desc;
		desc.Name = "CoCShader";
		desc.VertexPath = "Content\\shaders\\postprocessing\\Fullscreen.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\CoC.frag";
		CoCshader = Device::CreateShader(desc);
	}
	{
		ShaderDescription desc;
		desc.Name = "CoCDownSampleShader";
		desc.VertexPath = "Content\\shaders\\postprocessing\\Fullscreen.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\CoCDownSample.frag";
		CoCDownSample = Device::CreateShader(desc);
	}
	{
		ShaderDescription desc;
		desc.Name = "BokehShader";
		desc.VertexPath = "Content\\shaders\\postprocessing\\Fullscreen.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\Bokeh.frag";
		BokehShader = Device::CreateShader(desc);
	}
	{
		ShaderDescription desc;
		desc.Name = "BokehBlurShader";
		desc.VertexPath = "Content\\shaders\\postprocessing\\Fullscreen.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\BokehBlur.frag";
		BokehBlurShader = Device::CreateShader(desc);
	}
	{
		ShaderDescription desc;
		desc.Name = "CombineShader";
		desc.VertexPath = "Content\\shaders\\postprocessing\\Fullscreen.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\DoFCombine.frag";
		DoFCombine = Device::CreateShader(desc);
	}
	{
		ResourceDescription desc;
		desc.Name = "DoFBuffer";
		desc.Type = ResourceType::RenderTarget;
		desc.HasDepthStencil = false;
		desc.minFilter = TextureMinFilter::Near;
		desc.magFilter = TextureMagFilter::Near;
		desc.ColorBufferInternalformats = { InternalFormat::RGBA16F };
		desc.Width = Device::ScreenSize().x / 2;
		desc.Height = Device::ScreenSize().x / 2;
		desc.TextureTarget = TextureTarget::Texture2D;
		desc.TextureAttachmentTarget = AttachmentTarget::Color;
		Context.AddRenderResource(desc);
	}
	{
		TextureDescription desc;
		desc.Name = "CoCTexture";
		desc.ID = UUID();
		desc.NoChannels = 1;
		desc.MipLevels = 1;
		desc.GenMinMaps = false;
		desc.formatType = FormatType::Float;
		desc.format = Format::Red;
		desc.internalFormat = InternalFormat::R16F;
		desc.TextureTarget = TextureTarget::Texture2D;
		desc.minFilter = TextureMinFilter::Linear;
		desc.magFilter = TextureMagFilter::Linear;
		desc.Wrap = TextureWrap::ClampToEdge;
		desc.Width = Device::ScreenSize().x;
		desc.Height = Device::ScreenSize().y;



		TextureLoader loader;
		mCoCTexture = loader.Load(desc);
		Device::CreateTexture(mCoCTexture.get());
	}
	{
		TextureDescription desc;
		desc.Name = "CoCDownsampledTexture";
		desc.ID = UUID();
		desc.NoChannels = 1;
		desc.MipLevels = 1;
		desc.GenMinMaps = false;
		desc.formatType = FormatType::Float;
		desc.format = Format::Red;
		desc.internalFormat = InternalFormat::R16F;
		desc.TextureTarget = TextureTarget::Texture2D;
		desc.minFilter = TextureMinFilter::Linear;
		desc.magFilter = TextureMagFilter::Linear;
		desc.Wrap = TextureWrap::ClampToEdge;
		desc.Width = Device::ScreenSize().x / 2;
		desc.Height = Device::ScreenSize().y /2;


		TextureLoader loader;
		mCoCDownSampledTexture = loader.Load(desc);
		Device::CreateTexture(mCoCDownSampledTexture.get());
	}
	{
		TextureDescription desc;
		desc.Name = "BokehTexture";
		desc.ID = UUID();
		desc.NoChannels = 1;
		desc.MipLevels = 1;
		desc.GenMinMaps = false;
		desc.formatType = FormatType::Float;
		desc.format = Format::RGBA;
		desc.internalFormat = InternalFormat::RGBA16F;
		desc.TextureTarget = TextureTarget::Texture2D;
		desc.minFilter = TextureMinFilter::Near;
		desc.magFilter = TextureMagFilter::Near;
		desc.Wrap = TextureWrap::ClampToEdge;
		desc.Width = Device::ScreenSize().x / 2;
		desc.Height = Device::ScreenSize().y / 2;


		TextureLoader loader;
		mBokehTexture = loader.Load(desc);
		Device::CreateTexture(mBokehTexture.get());
	}
	{
		TextureDescription desc;
		desc.Name = "BokehBlurTexture";
		desc.ID = UUID();
		desc.NoChannels = 1;
		desc.MipLevels = 1;
		desc.GenMinMaps = false;
		desc.formatType = FormatType::Float;
		desc.format = Format::RGBA;
		desc.internalFormat = InternalFormat::RGBA16F;
		desc.TextureTarget = TextureTarget::Texture2D;
		desc.minFilter = TextureMinFilter::Linear;
		desc.magFilter = TextureMagFilter::Linear;
		desc.Wrap = TextureWrap::ClampToEdge;
		desc.Width = Device::ScreenSize().x / 2;
		desc.Height = Device::ScreenSize().y / 2;


		TextureLoader loader;
		mBokehBlurTexture = loader.Load(desc);
		Device::CreateTexture(mBokehBlurTexture.get());
	}
	
}

void Fracture::DepthOfFieldNode::Run(Scene& scene, CameraComponent* camera)
{	
	if(camera->EnableDepthOfField)
	{
		
		const auto& depth_texture = Context.GetRenderReadTarget("BackBufferColor")->DepthStencil->RenderID;
		const auto& coc_texture = mCoCTexture->RenderID;
			
		const auto& write = Context.GetRenderWriteTarget("DoFBuffer");
		Context.BindRenderTarget(write->RenderID);
		Context.BindShader(CoCshader->Program);
		{
			//Context.ClearBuffers((uint32_t)GLClearBufferBit::Color);
			Context.SetUniforms([=]() {CoCshader->SetUniform("FocalLength", camera->FocalLength); });
			Context.SetUniforms([=]() {CoCshader->SetUniform("FocalRange", camera->FocalRange); });
			Context.SetUniforms([=]() {CoCshader->SetUniform("near", camera->Near); });
			Context.SetUniforms([=]() {CoCshader->SetUniform("far", camera->Far); });
			Context.SetUniforms([=]()
				{
					CoCshader->SetTexture("depth_Texture", depth_texture, 0);
					CoCshader->SetUniform("BokehRadius", Properties.BokehRadius);
				});

			Context.SetUniforms([=]() {
				glNamedFramebufferTexture(write->RenderID, GL_COLOR_ATTACHMENT0, coc_texture, 0);
				});

			Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
			Context.DrawArrays(DrawMode::Triangle, 0, 3);
		}
		Context.BindShader(0);
		
		Context.BindRenderTarget(0);
		{
			const auto& write = Context.GetRenderWriteTarget("DoFBuffer");

			Context.SetViewport(0, 0, mBokehTexture->Description.Width, mBokehTexture->Description.Height);
			Context.BindRenderTarget(write->RenderID);
			Context.BindShader(CoCDownSample->Program);
			{

				Context.SetUniforms([=]()
					{
						CoCDownSample->SetTexture("main_Texture", mCoCTexture->RenderID, 0);
						CoCDownSample->SetUniform("BokehRadius", Properties.BokehRadius);
					});



				Context.SetUniforms([=]() {
					glNamedFramebufferTexture(write->RenderID, GL_COLOR_ATTACHMENT0, mCoCDownSampledTexture->RenderID, 0);
					});

				Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
				Context.DrawArrays(DrawMode::Triangle, 0, 3);
			}

		}


		{
			const auto& write = Context.GetRenderWriteTarget("DoFBuffer");
			const auto& main_texture = Context.PostProcessReadTarget()->ColorAttachments[0]->RenderID;			
			
			Context.SetViewport(0, 0, mBokehTexture->Description.Width, mBokehTexture->Description.Height);


			Context.BindRenderTarget(write->RenderID);
			Context.BindShader(BokehShader->Program);
			{

				Context.SetUniforms([=]()
					{
						BokehShader->SetTexture("main_Texture", main_texture, 0);
						BokehShader->SetTexture("coc_Texture", mCoCDownSampledTexture->RenderID, 1);
						BokehShader->SetUniform("BokehRadius", Properties.BokehRadius);
					});



				Context.SetUniforms([=]() {
					glNamedFramebufferTexture(write->RenderID, GL_COLOR_ATTACHMENT0, mBokehTexture->RenderID, 0);
					});

				Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
				Context.DrawArrays(DrawMode::Triangle, 0, 3);
			}

		}

		

		//BOkeh BLur
		{
			const auto& write = Context.GetRenderWriteTarget("DoFBuffer");

			Context.BindRenderTarget(write->RenderID);
			Context.BindShader(BokehBlurShader->Program);
			{

				Context.SetUniforms([=]()
					{
						BokehBlurShader->SetTexture("main_Texture", mBokehTexture->RenderID, 0);
					});



				Context.SetUniforms([=]() {
					glNamedFramebufferTexture(write->RenderID, GL_COLOR_ATTACHMENT0, mBokehBlurTexture->RenderID, 0);
					});

				Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
				Context.DrawArrays(DrawMode::Triangle, 0, 3);
			}

		}

		Context.BindShader(0);
		Context.BindRenderTarget(0);

		//Combine
		Context.SetViewport(0, 0, Context.GetRenderReadTarget("BackBufferColor")->ColorAttachments[0]->Description.Width, Context.GetRenderReadTarget("BackBufferColor")->ColorAttachments[0]->Description.Height);
		{
			const auto& main_texture = Context.PostProcessReadTarget()->ColorAttachments[0]->RenderID;
			const auto& write = Context.PostProcessWriteTarget();// Context.GetRenderWriteTarget("DoFBuffer");
	
			Context.BindRenderTarget(write->RenderID);
			Context.BindShader(DoFCombine->Program);
			{

				Context.SetUniforms([=]()
					{
						DoFCombine->SetTexture("main_Texture", main_texture, 0);
						DoFCombine->SetTexture("coc_Texture", mCoCDownSampledTexture->RenderID, 1);
						DoFCombine->SetTexture("bokeh_Texture", mBokehBlurTexture->RenderID, 2);
					});

				Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
				Context.DrawArrays(DrawMode::Triangle, 0, 3);
			}

		}

		Context.BindShader(0);
		Context.BindRenderTarget(0);
	}

	
}

#include "pch.h"
#include "BloomNode.h"
#include "core/Application.h"
#include "core/GameWindow.h"
#include "rendering/Device.h"
#include "rendering/Pipeline.h"
#include "assets/TextureLoader.h"

Fracture::BloomNode::BloomNode(DeviceContext& context, const  std::string& name, const std::vector<std::string>& inputs, const std::vector<std::string>& outputs) :RenderNode(context, name, inputs, outputs)
{
}

void Fracture::BloomNode::Setup()
{
	int  mipChainLength = 5;
	glm::vec2 mipSize((float)1920, (float)1080);
	glm::ivec2 mipIntSize((int)1920, (int)1080);

	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\postprocessing\\Fullscreen.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\DownScale.frag";
		downsampleShader = Device::CreateShader(desc);
	}
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\postprocessing\\Fullscreen.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\UpScale.frag";
		upsampleShader = Device::CreateShader(desc);
	}
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\postprocessing\\Fullscreen.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\BloomFinal.frag";
		bloomfinal = Device::CreateShader(desc);
	}
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\postprocessing\\Fullscreen.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\BrightPass.frag";
		brightpassShader = Device::CreateShader(desc);
	}
	{
		TextureDescription desc;
		desc.format = Format::RGBA;
		desc.formatType = FormatType::UByte;
		desc.internalFormat = InternalFormat::RGBA;
		desc.Path = "Content\\textures\\DirtMask.png";
		TextureLoader loader;
		mDirtMask = loader.Load(desc);
		mDirtMask->Name = "Grass";
		Device::CreateTexture(mDirtMask.get());
	}
	{
		RenderTargetDescription desc;
		desc.Name = "BloomOut";
		mBloomFBO = Device::CreateRenderTarget(desc);

		Device::BeginRenderTargetSetup(mBloomFBO.get());
		for (unsigned int i = 0; i < mipChainLength; i++)
		{
			std::shared_ptr<BloomMipMap> mip = std::make_shared<BloomMipMap>();
			mipSize *= 0.5f;
			mipIntSize /= 2;
			mip->size = mipSize;
			mip->isize = mipIntSize;

			TextureDescription tdesc;
			tdesc.TextureTarget = TextureTarget::Texture2D;
			tdesc.AttachmentTrgt = AttachmentTarget::Color;
			tdesc.format = Format::RGBA;
			tdesc.formatType = FormatType::Float;
			tdesc.internalFormat = InternalFormat::RGBA16F;
			tdesc.minFilter = TextureMinFilter::Linear;
			tdesc.magFilter = TextureMagFilter::Linear;
			tdesc.Wrap = TextureWrap::ClampToEdge;
			tdesc.Width = (int)mip->size.x;
			tdesc.Height = (int)mip->size.y;
			mip->texture = Device::CreateTexture(tdesc);
			BloomMipMaps.push_back(mip);
		}
		Device::AddTextureToRenderTarget(AttachmentTarget::Color, TextureTarget::Texture2D, BloomMipMaps[0]->texture->RenderID);

		Device::EndRenderTargetSetup();
		AddResource(std::move(mBloomFBO));
	}

}

void Fracture::BloomNode::Run(Scene& scene, CameraComponent* camera)
{	
	const  auto& mipChain = BloomMipMaps;

	//Bright Pass
	if (Properties.UseBrightPass)
	{
		const auto& read = Context.GetRenderReadTarget("FXAA")->ColorAttachments[0]->RenderID;
		//const auto& read = Context.PostProcessReadTarget()->ColorAttachments[0]->RenderID;
		const auto& write = Context.PostProcessWriteTarget();
		Context.BindRenderTarget(write->RenderID);
		Context.BindShader(brightpassShader->Program);
		{
			Context.SetUniforms([=]() {brightpassShader->SetUniform("brightPassThreshold", Properties.Threshold); });			
			Context.SetUniforms([=]()
				{
					brightpassShader->SetTexture("srcTexture", read, 0);					
				});
			Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
			Context.DrawArrays(DrawMode::Triangle, 0, 3);
		}
		Context.BindShader(0);
		Context.BindRenderTarget(0);
	}
	
	

	///Down Sample	
	{
		uint32_t read;
		if(Properties.UseBrightPass)
			read = Context.PostProcessReadTarget()->ColorAttachments[0]->RenderID;		
		else
			read = Context.GetRenderReadTarget("FXAA")->ColorAttachments[0]->RenderID;

		const auto& write = Context.GetRenderWriteTarget("BloomOut");
		const auto& emission = Context.GetRenderReadTarget("BackBufferColor")->ColorAttachments[4].get();
		//const auto& emissive = Context.GetRenderReadTarget("BackBufferColor")->ColorAttachments[4]->RenderID;

		Context.BindRenderTarget(write->RenderID);
		//Context.SetViewport(0, 0, Context.GetViewSize().x, Context.GetViewSize().y);		
		Context.BindShader(downsampleShader->Program);
		Context.SetUniforms([=]() {downsampleShader->SetUniform("srcResolution", Context.GetViewSize()); });
		Context.SetUniforms([=]() {downsampleShader->SetUniform("EmissionStrength", Properties.EmissionStrength); });
		Context.SetUniforms([=]()
			{
				downsampleShader->SetTexture("emissionTexture", emission->RenderID, 1);
				downsampleShader->SetTexture("srcTexture", read,0);		
			});
		
		for (int i = 0; i < mipChain.size(); i++)
		{
			const auto& mip = mipChain[i];
			Context.SetViewport(0, 0, mip->size.x, mip->size.y);

			auto texture = mip->texture->RenderID;

			

			Context.SetUniforms([=]() {			
				glNamedFramebufferTexture(write->RenderID, GL_COLOR_ATTACHMENT0, texture, 0);
				});
			
			Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
			Context.DrawArrays(DrawMode::Triangle, 0, 3);

			auto mipsize = mip->size;
			Context.SetUniforms([=]() {downsampleShader->SetUniform("mipLevel",i); });
			Context.SetUniforms([=]() {downsampleShader->SetUniform("srcResolution", mipsize); });
			Context.SetUniforms([=]()
				{
					downsampleShader->SetTexture("srcTexture", texture, 0);
				});
		}
	}

	///Up Sample
	{
		const auto& write = Context.GetRenderWriteTarget("BloomOut");

		Context.BindShader(upsampleShader->Program);
		Context.SetUniforms([=]() {upsampleShader->SetUniform("filterRadius", Properties.FilterRadius); });


		// Enable additive blending		
		Context.SetUniforms([=]() 
			{
				glDisable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_ONE, GL_ONE);
				glBlendEquation(GL_FUNC_ADD);
			});

		for (int i = mipChain.size() - 1; i > 0; i--)
		{
			const auto& mip = mipChain[i];
			const auto& nextMip = mipChain[i - 1];
			Context.SetViewport(0, 0, nextMip->size.x, nextMip->size.y);

			auto mipTexture = mip->texture->RenderID;
			Context.SetUniforms([=]()
				{
					upsampleShader->SetTexture("srcTexture", mipTexture, 0);
				});
			
			auto texture = nextMip->texture->RenderID;
			Context.SetUniforms([=]() {
				glNamedFramebufferTexture(write->RenderID, GL_COLOR_ATTACHMENT0, texture, 0);
				});



			Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
			Context.DrawArrays(DrawMode::Triangle, 0, 3);
		}

		
		Context.BindShader(0);
		Context.BindRenderTarget(0);
		Context.SetUniforms([=]()
			{
				glDisable(GL_BLEND);
			});
		Context.BindShader(0);
		Context.BindRenderTarget(0);
	}

	{	
		Context.SetViewport(0, 0, Context.GetRenderReadTarget("BackBufferColor")->ColorAttachments[0]->Description.Width, Context.GetRenderReadTarget("BackBufferColor")->ColorAttachments[0]->Description.Height);

		const auto& read = Context.PostProcessReadTarget()->ColorAttachments[0]->RenderID;
		const auto& write = Context.PostProcessWriteTarget();
	
		Context.BindRenderTarget(write->RenderID);
		Context.BindShader(bloomfinal->Program);
		
	
		const auto& backbuffer = Context.GetRenderReadTarget("FXAA")->ColorAttachments[0]->RenderID;

		Context.SetUniforms([=]() {bloomfinal->SetUniform("BloomStrength", Properties.bloomStrength); });
		Context.SetUniforms([=]() {bloomfinal->SetUniform("dirtMaskIntensity", Properties.DirtStrength); });

		const auto& mip = mipChain[0];

		Context.SetUniforms([=]()
			{				
				bloomfinal->SetTexture("srcTexture", backbuffer, 0);

				bloomfinal->SetTexture("bloomTexture", mip->texture->RenderID, 1);

				if (mDirtMask)
					bloomfinal->SetTexture("DirtMask", mDirtMask->RenderID, 2);

				
			});
		Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
		Context.DrawArrays(DrawMode::Triangle, 0, 3);
	}

	
}

void Fracture::BloomNode::ReloadShader()
{
	Device::ReloadShader(bloomfinal.get());
	Device::ReloadShader(upsampleShader.get());
	Device::ReloadShader(downsampleShader.get());
	Device::ReloadShader(brightpassShader.get());
}

const std::vector<std::shared_ptr<Fracture::BloomMipMap>>& Fracture::BloomNode::MipChain() const
{
	return Fracture::BloomNode::BloomMipMaps;
}

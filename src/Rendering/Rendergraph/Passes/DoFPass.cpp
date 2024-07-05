#include "FracturePCH.h"
#include "DoFPass.h"

void Fracture::DoFPass::Setup()
{
	CoCshader = AssetManager::Instance()->GetShader("CoC");
	CoCDownSample = AssetManager::Instance()->GetShader("CoCDownSample");
	BokehShader = AssetManager::Instance()->GetShader("DoFBokeh");
	BokehBlurShader = AssetManager::Instance()->GetShader("BokehBlur");
	DoFCombine = AssetManager::Instance()->GetShader("DoFCombine");


	{
		RenderTargetCreationInfo desc;
		desc.Name = "DoFBuffer";
		desc.ColorAttachments.resize(1);			
		desc.ColorAttachments[0].minFilter = TextureMinFilter::Near;
		desc.ColorAttachments[0].magFilter = TextureMagFilter::Near;
		desc.ColorAttachments[0].internalFormat = InternalFormat::RGBA16F;
		desc.ColorAttachments[0].Width = GraphicsDevice::Instance()->Viewport_Width;
		desc.ColorAttachments[0].Height = GraphicsDevice::Instance()->Viewport_Height;
		desc.ColorAttachments[0].TextureTarget = TextureTarget::Texture2D;
		desc.ColorAttachments[0].AttachmentTrgt = AttachmentTarget::Color;
		mDoFTarget = GraphicsDevice::Instance()->CreateRenderTarget(desc);
	}
	{
		TextureCreationInfo desc;
		desc.Name = "CoCTexture";
		desc.ID = UUID();
		desc.NoChannels = 1;
		desc.MipLevels = 1;
		desc.GenMinMaps = false;
		desc.formatType = TextureFormatType::Float;
		desc.format = TextureFormat::Red;
		desc.internalFormat = InternalFormat::R16F;
		desc.TextureTarget = TextureTarget::Texture2D;
		desc.minFilter = TextureMinFilter::Linear;
		desc.magFilter = TextureMagFilter::Linear;
		desc.Wrap = TextureWrap::ClampToEdge;
		desc.Width = GraphicsDevice::Instance()->Viewport_Width;
		desc.Height = GraphicsDevice::Instance()->Viewport_Height;
		mCoCTexture = std::make_shared<Texture>(desc);
		GraphicsDevice::Instance()->CreateTexture(mCoCTexture,desc);
	}
	{
		TextureCreationInfo desc;
		desc.Name = "CoCDownsampledTexture";
		desc.ID = UUID();
		desc.NoChannels = 1;
		desc.MipLevels = 1;
		desc.GenMinMaps = false;
		desc.formatType = TextureFormatType::Float;
		desc.format = TextureFormat::Red;
		desc.internalFormat = InternalFormat::R16F;
		desc.TextureTarget = TextureTarget::Texture2D;
		desc.minFilter = TextureMinFilter::Linear;
		desc.magFilter = TextureMagFilter::Linear;
		desc.Wrap = TextureWrap::ClampToEdge;
		desc.Width = GraphicsDevice::Instance()->Viewport_Width;
		desc.Height = GraphicsDevice::Instance()->Viewport_Height ;
		mCoCDownSampledTexture = std::make_shared<Texture>(desc);
		GraphicsDevice::Instance()->CreateTexture(mCoCDownSampledTexture, desc);
	}
	{
		TextureCreationInfo desc;
		desc.Name = "BokehTexture";
		desc.ID = UUID();
		desc.NoChannels = 1;
		desc.MipLevels = 1;
		desc.GenMinMaps = false;
		desc.formatType = TextureFormatType::Float;
		desc.format = TextureFormat::RGBA;
		desc.internalFormat = InternalFormat::RGBA16F;
		desc.TextureTarget = TextureTarget::Texture2D;
		desc.minFilter = TextureMinFilter::Near;
		desc.magFilter = TextureMagFilter::Near;
		desc.Wrap = TextureWrap::ClampToEdge;
		desc.Width = GraphicsDevice::Instance()->Viewport_Width;
		desc.Height = GraphicsDevice::Instance()->Viewport_Height;
		mBokehTexture = std::make_shared<Texture>(desc);
		GraphicsDevice::Instance()->CreateTexture(mBokehTexture, desc);
	}
	{
		TextureCreationInfo desc;
		desc.Name = "BokehBlurTexture";
		desc.ID = UUID();
		desc.NoChannels = 1;
		desc.MipLevels = 1;
		desc.GenMinMaps = false;
		desc.formatType = TextureFormatType::Float;
		desc.format = TextureFormat::RGBA;
		desc.internalFormat = InternalFormat::RGBA16F;
		desc.TextureTarget = TextureTarget::Texture2D;
		desc.minFilter = TextureMinFilter::Linear;
		desc.magFilter = TextureMagFilter::Linear;
		desc.Wrap = TextureWrap::ClampToEdge;
		desc.Width = GraphicsDevice::Instance()->Viewport_Width;
		desc.Height = GraphicsDevice::Instance()->Viewport_Height;
		mBokehBlurTexture = std::make_shared<Texture>(desc);
		GraphicsDevice::Instance()->CreateTexture(mBokehBlurTexture, desc);
	}

}

bool Fracture::DoFPass::Render(PostProcessInfo info)
{
	OPTICK_EVENT();
	if (!GraphicsDevice::Instance()->RenderSettings.DoFEnabled)
		return false;
	
	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalColour);
	
	RenderCommands::SetViewport(info.cntxt, GraphicsDevice::Instance()->Viewport_Width, GraphicsDevice::Instance()->Viewport_Height);
	//CoC Pass
	{
		RenderCommands::SetRenderTarget(info.cntxt, mDoFTarget->Handle);
		RenderCommands::UseProgram(info.cntxt, CoCshader->Handle);
		RenderCommands::SetTexture(info.cntxt, CoCshader.get(), "InDepth", global_color->DepthStencilAttachment->Handle, 0);
		RenderCommands::FrameBufferAttachTexture(info.cntxt, mDoFTarget->Handle, 0,mCoCTexture->Handle, 0);

		DrawArray cmd =
		{
			.mode = DrawMode::Triangles,
			.first = 0,
			.count = 3
		};
		RenderCommands::BindVertexArrayObject(info.cntxt, info.VAO);
		RenderCommands::DrawArray(info.cntxt, cmd);
		Fracture::RenderCommands::ResetTextureUnits(info.cntxt, CoCshader.get());
		Fracture::RenderCommands::ReleaseRenderTarget(info.cntxt);
		Fracture::RenderCommands::UseProgram(info.cntxt, 0);
	}

	//Bokeh Downsample Pass
	{
		RenderCommands::SetRenderTarget(info.cntxt, mDoFTarget->Handle);
		RenderCommands::SetViewport(info.cntxt, mBokehTexture->Description.Width, mBokehTexture->Description.Height);
		RenderCommands::UseProgram(info.cntxt, CoCDownSample->Handle);	
		RenderCommands::SetTexture(info.cntxt, CoCDownSample.get(), "InColor", mCoCTexture->Handle, 0);

		RenderCommands::FrameBufferAttachTexture(info.cntxt, mDoFTarget->Handle, 0, mCoCDownSampledTexture->Handle, 0);
			
		DrawArray cmd =
		{
			.mode = DrawMode::Triangles,
			.first = 0,
			.count = 3
		};
		RenderCommands::BindVertexArrayObject(info.cntxt, info.VAO);
		RenderCommands::DrawArray(info.cntxt, cmd);
		Fracture::RenderCommands::ResetTextureUnits(info.cntxt, CoCDownSample.get());
		Fracture::RenderCommands::ReleaseRenderTarget(info.cntxt);
		Fracture::RenderCommands::UseProgram(info.cntxt, 0);
	}

	//Bokeh Pass
	{
		RenderCommands::SetRenderTarget(info.cntxt, mDoFTarget->Handle);
		RenderCommands::SetViewport(info.cntxt, mBokehTexture->Description.Width, mBokehTexture->Description.Height);
		RenderCommands::UseProgram(info.cntxt, BokehShader->Handle);			

		RenderCommands::SetTexture(info.cntxt, BokehShader.get(), "InColor", info.input_texture, 0);
		RenderCommands::SetTexture(info.cntxt, BokehShader.get(), "InCoC", mCoCDownSampledTexture->Handle, 1);
		
		RenderCommands::FrameBufferAttachTexture(info.cntxt, mDoFTarget->Handle, 0, mBokehTexture->Handle, 0);

		DrawArray cmd =
		{
			.mode = DrawMode::Triangles,
			.first = 0,
			.count = 3
		};
		RenderCommands::BindVertexArrayObject(info.cntxt, info.VAO);
		RenderCommands::DrawArray(info.cntxt, cmd);
		Fracture::RenderCommands::ResetTextureUnits(info.cntxt, BokehShader.get());
		Fracture::RenderCommands::ReleaseRenderTarget(info.cntxt);
		Fracture::RenderCommands::UseProgram(info.cntxt, 0);
	}

	//BOkeh BLur
	{
		RenderCommands::SetRenderTarget(info.cntxt, mDoFTarget->Handle);
		RenderCommands::SetViewport(info.cntxt, mBokehTexture->Description.Width, mBokehTexture->Description.Height);
		RenderCommands::UseProgram(info.cntxt, BokehBlurShader->Handle);

		RenderCommands::SetTexture(info.cntxt, BokehBlurShader.get(), "InColor", mBokehTexture->Handle, 0);

		RenderCommands::FrameBufferAttachTexture(info.cntxt, mDoFTarget->Handle, 0, mCoCDownSampledTexture->Handle, 0);

		DrawArray cmd =
		{
			.mode = DrawMode::Triangles,
			.first = 0,
			.count = 3
		};
		RenderCommands::BindVertexArrayObject(info.cntxt, info.VAO);
		RenderCommands::DrawArray(info.cntxt, cmd);
		Fracture::RenderCommands::ResetTextureUnits(info.cntxt, BokehBlurShader.get());
		Fracture::RenderCommands::ReleaseRenderTarget(info.cntxt);
		Fracture::RenderCommands::UseProgram(info.cntxt, 0);
	}

	//BokehCombine
	{
		RenderCommands::SetRenderTarget(info.cntxt, info.write_to);
		RenderCommands::SetViewport(info.cntxt, GraphicsDevice::Instance()->Viewport_Width, GraphicsDevice::Instance()->Viewport_Height);
		RenderCommands::UseProgram(info.cntxt, DoFCombine->Handle);
		RenderCommands::SetTexture(info.cntxt, DoFCombine.get(), "InColor", info.input_texture, 0);
		RenderCommands::SetTexture(info.cntxt, DoFCombine.get(), "InCoC", mCoCTexture->Handle, 1);
		RenderCommands::SetTexture(info.cntxt, DoFCombine.get(), "InBokeh", mBokehTexture->Handle, 2);


		DrawArray cmd =
		{
			.mode = DrawMode::Triangles,
			.first = 0,
			.count = 3
		};

		RenderCommands::BindVertexArrayObject(info.cntxt, info.VAO);
		RenderCommands::DrawArray(info.cntxt, cmd);
		Fracture::RenderCommands::ResetTextureUnits(info.cntxt, DoFCombine.get());
		Fracture::RenderCommands::ReleaseRenderTarget(info.cntxt);
		Fracture::RenderCommands::UseProgram(info.cntxt, 0);
		return true;

	}

	RenderCommands::SetRenderTarget(info.cntxt, info.write_to);
	
	return true;
}

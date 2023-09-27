#include "FracturePCH.h"
#include "BloomPass.h"
#include "Assets/AssetManager.h"



Fracture::BloomPass::BloomPass():IPostProcess()
{
}

void Fracture::BloomPass::Setup()
{
	int  mipChainLength = 8;
	glm::vec2 mipSize((float)1920, (float)1080);
	glm::ivec2 mipIntSize((int)1920, (int)1080);

	
	{
		RenderTargetCreationInfo info;
		info.Name = "BloomOut";
		info.HasRenderBuffer = true;
		info.Width = 1920;
		info.Height = 1080;
		for (unsigned int i = 0; i < mipChainLength; i++)
		{
			std::shared_ptr<BloomMipMap> mip = std::make_shared<BloomMipMap>();
			mipSize *= 0.5f;
			mipIntSize /= 2;
			mip->size = mipSize;
			mip->isize = mipIntSize;

			TextureCreationInfo desc;
			desc.TextureTarget = TextureTarget::Texture2D;
			desc.AttachmentTrgt = AttachmentTarget::Color;
			desc.format = TextureFormat::RGBA;
			desc.formatType = TextureFormatType::Float;
			desc.internalFormat = InternalFormat::RGBA16F;
			desc.minFilter = TextureMinFilter::Linear;
			desc.magFilter = TextureMagFilter::Linear;
			desc.Wrap = TextureWrap::ClampToEdge;
			desc.Width = (int)mip->size.x;
			desc.Height = (int)mip->size.y;
			mip->texture = std::make_shared <Texture>(desc);
			GraphicsDevice::Instance()->CreateTexture(mip->texture, desc);
			BloomMipMaps.push_back(mip);
		}
	
		mBloomFBO = GraphicsDevice::Instance()->CreateRenderTarget(info);

		{
			TextureCreationInfo desc;
			desc.TextureTarget = TextureTarget::Texture2D;
			desc.AttachmentTrgt = AttachmentTarget::Color;
			desc.format = TextureFormat::RGBA;
			desc.formatType = TextureFormatType::Float;
			desc.internalFormat = InternalFormat::RGBA16F;
			desc.minFilter = TextureMinFilter::Linear;
			desc.magFilter = TextureMagFilter::Linear;
			desc.Wrap = TextureWrap::ClampToEdge;
			desc.Width = (int)1920;
			desc.Height = (int)1080;
			mPrefilteredTexture = std::make_shared <Texture>(desc);
			GraphicsDevice::Instance()->CreateTexture(mPrefilteredTexture, desc);
		}
	
	}
	{
		BloomPrefilterShader = AssetManager::GetShader("BloomPrefilter");
		downsampleShader = AssetManager::GetShader("BloomDownSample");
		upsampleShader = AssetManager::GetShader("BloomUpSample");
		bloomfinal = AssetManager::GetShader("BloomFinal");
	}
}
bool Fracture::BloomPass::Render(PostProcessInfo info)
{
	OPTICK_EVENT();
	
	if (!GraphicsDevice::Instance()->RenderSettings.BloomEnabled)
		return false;

	const  auto& mipChain = BloomMipMaps;

	RenderCommands::Disable(info.cntxt, Fracture::GLCapability::Blending);

	//Bright Pass
	{
		RenderCommands::SetRenderTarget(info.cntxt, mBloomFBO->Handle);
		RenderCommands::UseProgram(info.cntxt, BloomPrefilterShader->Handle);
		

		Fracture::RenderCommands::FrameBufferAttachTexture(info.cntxt, mBloomFBO->Handle, 0, mPrefilteredTexture->Handle, 0);
		Fracture::RenderCommands::SetTexture(info.cntxt, BloomPrefilterShader.get(), "srcTexture", info.input_texture, 0);
		
		DrawArray cmd =
		{
			.mode = DrawMode::Triangles,
			.first = 0,
			.count = 3
		};

		RenderCommands::BindVertexArrayObject(info.cntxt, info.VAO);
		RenderCommands::DrawArray(info.cntxt, cmd);
	}

	///Down Sample	
	{
		RenderCommands::SetRenderTarget(info.cntxt, mBloomFBO->Handle);
		RenderCommands::UseProgram(info.cntxt, downsampleShader->Handle);		

		Fracture::RenderCommands::SetUniform(info.cntxt, downsampleShader.get(), "srcResolution", glm::vec2(1920,1080));
		Fracture::RenderCommands::SetTexture(info.cntxt, downsampleShader.get(), "srcTexture", mPrefilteredTexture->Handle,0);
		

		for (int i = 0; i < mipChain.size(); i++)
		{
			const auto& mip = mipChain[i];
			Fracture::RenderCommands::SetViewport(info.cntxt, mip->size.x, mip->size.y);
			Fracture::RenderCommands::FrameBufferAttachTexture(info.cntxt, mBloomFBO->Handle, 0, mip->texture->Handle, 0);

			DrawArray cmd =
			{
				.mode = DrawMode::Triangles,
				.first = 0,
				.count = 3
			};

			RenderCommands::BindVertexArrayObject(info.cntxt, info.VAO);
			RenderCommands::DrawArray(info.cntxt, cmd);

			auto mipsize = mip->size;
			Fracture::RenderCommands::SetUniform(info.cntxt, downsampleShader.get(), "mipLevel", i);
			Fracture::RenderCommands::SetUniform(info.cntxt, downsampleShader.get(), "srcResolution", mipsize);
			Fracture::RenderCommands::SetTexture(info.cntxt, downsampleShader.get(), "srcTexture", mip->texture->Handle,0);
		}
	}

	///Up Sample
	{
		const auto& write = mBloomFBO->Handle;
		RenderCommands::UseProgram(info.cntxt, upsampleShader->Handle);		
		RenderCommands::Enable(info.cntxt, Fracture::GLCapability::Blending);
		RenderCommands::Disable(info.cntxt, Fracture::GLCapability::DepthTest);
		RenderCommands::BlendFunction(info.cntxt, Fracture::BlendFunc::One , Fracture::BlendFunc::One);
		RenderCommands::BlendEquation(info.cntxt, Fracture::BlendEq::Func_ADD);		
		
		for (int i = mipChain.size() - 1; i > 0; i--)
		{
			const auto& mip = mipChain[i];
			const auto& nextMip = mipChain[i - 1];
			Fracture::RenderCommands::SetViewport(info.cntxt, nextMip->size.x, nextMip->size.y, 0, 0);

			auto mipTexture = mip->texture->Handle;
			Fracture::RenderCommands::SetTexture(info.cntxt, upsampleShader.get(), "srcTexture", mipTexture, 0);

			Fracture::RenderCommands::FrameBufferAttachTexture(info.cntxt, write, 0, nextMip->texture->Handle, 0);
			DrawArray cmd =
			{
				.mode = DrawMode::Triangles,
				.first = 0,
				.count = 3
			};

			RenderCommands::BindVertexArrayObject(info.cntxt, info.VAO);
			RenderCommands::DrawArray(info.cntxt, cmd);
		}		
		RenderCommands::Disable(info.cntxt, Fracture::GLCapability::Blending);
	}

	//Bloom Final
	{
		Fracture::RenderCommands::SetViewport(info.cntxt, 1920, 1080);

		const auto& read = info.input_texture;
		const auto& write = info.write_to;

		RenderCommands::SetRenderTarget(info.cntxt, write);
		RenderCommands::UseProgram(info.cntxt, bloomfinal->Handle);

		const auto& mip = mipChain[0];
		Fracture::RenderCommands::SetTexture(info.cntxt, bloomfinal.get(), "srcTexture", read, 0);
		Fracture::RenderCommands::SetTexture(info.cntxt, bloomfinal.get(), "bloomTexture", mip->texture->Handle, 1);
		
		DrawArray cmd =
		{
			.mode = DrawMode::Triangles,
			.first = 0,
			.count = 3
		};

		RenderCommands::BindVertexArrayObject(info.cntxt, info.VAO);
		RenderCommands::DrawArray(info.cntxt, cmd);
	}

	return true;
}

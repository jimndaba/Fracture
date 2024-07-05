#include "FracturePCH.h"
#include "SSRPass.h"


Fracture::SSRPass::SSRPass(const std::string& name, RenderContext* context):IPass(name,context)
{
}

void Fracture::SSRPass::Setup()
{
	{
		VertexArrayCreationInfo info;
		GraphicsDevice::Instance()->CreateVertexArray(Vao, info);
	}
	{
		Fracture::RenderTargetCreationInfo info;
		{
			Fracture::TextureCreationInfo desc;
			desc.Width = GraphicsDevice::RenderSettings.SSR_Resolution.x;
			desc.Height = GraphicsDevice::RenderSettings.SSR_Resolution.y;
			desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
			desc.format = Fracture::TextureFormat::RGBA;
			desc.internalFormat = Fracture::InternalFormat::RGBA16F;
			desc.formatType = Fracture::TextureFormatType::Float;
			desc.minFilter = TextureMinFilter::Near;
			desc.magFilter = TextureMagFilter::Near;
			desc.Name = Fracture::GlobalRenderTargets::GlobalSSR;
			info.ColorAttachments.push_back(desc);
		}
		GraphicsDevice::Instance()->CreateGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalSSR, info);
	}
	{
		mSSR_Shader =AssetManager::GetShader("SSR");
	}

}

void Fracture::SSRPass::Execute()
{

	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalColour);
	const auto& global_ssr = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalSSR);

	if (!global_color)
		return;
	{
		Fracture::RenderCommands::SetRenderTarget(Context, global_ssr);

		RenderCommands::SetCullMode(Context, CullMode::None);
		RenderCommands::SetViewport(Context, GraphicsDevice::RenderSettings.SSR_Resolution.x, GraphicsDevice::RenderSettings.SSR_Resolution.y, 0, 0);
		RenderCommands::SetScissor(Context, GraphicsDevice::RenderSettings.SSR_Resolution.x, GraphicsDevice::RenderSettings.SSR_Resolution.y, 0, 0);
		RenderCommands::ClearColor(Context, Colour::Black);
		RenderCommands::ClearTarget(Context, (uint32_t)Fracture::ClearFlags::Color | (uint32_t)Fracture::ClearFlags::Depth);


		Fracture::RenderCommands::UseProgram(Context, mSSR_Shader->Handle);
		Fracture::RenderCommands::SetTexture(Context, mSSR_Shader.get(), "InNormal", global_color->ColorAttachments[(int)GlobalColorAttachments::Normal]->Handle, 0);
		Fracture::RenderCommands::SetTexture(Context, mSSR_Shader.get(), "InPosition", global_color->ColorAttachments[(int)GlobalColorAttachments::Position]->Handle, 1);
		Fracture::RenderCommands::SetTexture(Context, mSSR_Shader.get(), "InColor_Spec", global_color->ColorAttachments[(int)GlobalColorAttachments::Color]->Handle, 2);
		Fracture::RenderCommands::SetTexture(Context, mSSR_Shader.get(), "InDepth", global_color->DepthStencilAttachment->Handle, 3);



		DrawArray cmd =
		{
			.mode = DrawMode::Triangles,
			.first = 0,
			.count = 3
		};

		RenderCommands::BindVertexArrayObject(Context, Vao);
		RenderCommands::DrawArray(Context, cmd);

		Fracture::RenderCommands::ReleaseRenderTarget(Context);
	}
}

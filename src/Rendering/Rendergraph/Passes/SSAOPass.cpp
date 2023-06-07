#include "FracturePCH.h"
#include "SSAOPass.h"
#include "../../GraphicsDevice.h"
#include "Common/FractureApp.h"
#include "Assets/AssetManager.h"


Fracture::SSAOPass::SSAOPass(const std::string& name, RenderContext* context):
	IPass(name,context)
{
}

void Fracture::SSAOPass::Setup()
{
	{
		VertexArrayCreationInfo info;
		GraphicsDevice::Instance()->CreateVertexArray(Vao, info);
	}
	{
		randomFloats = std::uniform_real_distribution<float>(0.0, 1.0);
		for (unsigned int i = 0; i < 320; ++i)
		{
			glm::vec4 sample(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator), 0
			);
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			float scale = (float)i / 320;
			scale = lerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}

		BufferDescription desc;
		desc.bufferType = BufferType::UniformBuffer;
		desc.data = ssaoKernel.data();
		desc.size = sizeof(ssaoKernel[0]) * ssaoKernel.size();
		desc.usage = BufferUsage::Static;
		desc.Name = "Kernel Buffer";
		mKernelBuffer = std::make_shared<Buffer>();
		GraphicsDevice::Instance()->CreateBuffer(mKernelBuffer.get(), desc);		
		GraphicsDevice::Instance()->SetBufferIndexRange(mKernelBuffer.get(), (uint32_t)ShaderUniformIndex::SSAO_Kernel, 0);
	}
	
	{
		Fracture::RenderTargetCreationInfo info;
		{
			Fracture::TextureCreationInfo desc;
			desc.Width = GraphicsDevice::RenderSettings.SSAO_Resolution.x;
			desc.Height = GraphicsDevice::RenderSettings.SSAO_Resolution.y;
			desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
			desc.format = Fracture::TextureFormat::RGB;
			desc.internalFormat = Fracture::InternalFormat::RGB16F;
			desc.formatType = Fracture::TextureFormatType::Float;
			desc.minFilter = TextureMinFilter::Near;
			desc.magFilter = TextureMagFilter::Near;
			desc.Name = "SSAO";
			info.ColorAttachments.push_back(desc);
		}		
		GraphicsDevice::Instance()->CreateGlobalRenderTarget("Global_SSAO", info);
	}
	{
		Fracture::RenderTargetCreationInfo info;
		{
			Fracture::TextureCreationInfo desc;
			desc.Width = GraphicsDevice::RenderSettings.SSAO_Resolution.x;
			desc.Height = GraphicsDevice::RenderSettings.SSAO_Resolution.y;
			desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
			desc.format = Fracture::TextureFormat::RGB;
			desc.internalFormat = Fracture::InternalFormat::RGB16F;
			desc.formatType = Fracture::TextureFormatType::Float;
			desc.minFilter = TextureMinFilter::Near;
			desc.magFilter = TextureMagFilter::Near;
			desc.Name = "SSAO_Pass1";
			info.ColorAttachments.push_back(desc);
		}
		mSSAOrt =  GraphicsDevice::Instance()->CreateRenderTarget(info);
	}


	{
		int noise_i = 64 * 64;
		std::vector<glm::vec3> ssaoNoise;
		for (unsigned int i = 0; i < noise_i; i++)
		{
			glm::vec3 noise(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				0.0f);
			ssaoNoise.push_back(noise);
		}

		Fracture::TextureCreationInfo desc;
		desc.Width = 64;
		desc.Height = 64;
		desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
		desc.format = Fracture::TextureFormat::RGBA;
		desc.internalFormat = Fracture::InternalFormat::RGBA16F;
		desc.formatType = Fracture::TextureFormatType::Float;
		desc.minFilter = TextureMinFilter::Near;
		desc.magFilter = TextureMagFilter::Near;
		desc.Wrap = TextureWrap::Repeat;
		desc.Name = "SSAO_Noise";
		desc.texture_data = &ssaoNoise[0];
		mNoise = std::make_shared<Texture>(desc);
		GraphicsDevice::Instance()->CreateTexture(mNoise, desc);
	}
	

	{
		mSSAO_Shader = AssetManager::GetShader("SSAO");
		mBlur_Shader = AssetManager::GetShader("SSAOBlur");
	}
	
}

void Fracture::SSAOPass::Execute()
{
	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget("Global_ColorBuffer");
	const auto& global_SSAO = GraphicsDevice::Instance()->GetGlobalRenderTarget("Global_SSAO");

	if (!global_color)
		return;
	{
		Fracture::RenderCommands::SetRenderTarget(Context, mSSAOrt.get());

		RenderCommands::SetCullMode(Context, CullMode::None);
		RenderCommands::SetViewport(Context, GraphicsDevice::RenderSettings.SSAO_Resolution.x, GraphicsDevice::RenderSettings.SSAO_Resolution.y, 0, 0);
		RenderCommands::SetScissor(Context, GraphicsDevice::RenderSettings.SSAO_Resolution.x, GraphicsDevice::RenderSettings.SSAO_Resolution.y, 0, 0);
		RenderCommands::ClearColor(Context, Colour::CornflourBlue);
		RenderCommands::ClearTarget(Context, (uint32_t)Fracture::ClearFlags::Color | (uint32_t)Fracture::ClearFlags::Depth);


		Fracture::RenderCommands::UseProgram(Context, mSSAO_Shader->Handle);
		Fracture::RenderCommands::SetTexture(Context, mSSAO_Shader.get(), "InNormal", global_color->ColorAttachments[(int)GlobalColorAttachments::Normal]->Handle, 0);
		Fracture::RenderCommands::SetTexture(Context, mSSAO_Shader.get(), "InPosition", global_color->ColorAttachments[(int)GlobalColorAttachments::Position]->Handle, 1);
		Fracture::RenderCommands::SetTexture(Context, mSSAO_Shader.get(), "InNoise", mNoise->Handle, 2);
		Fracture::RenderCommands::SetTexture(Context, mSSAO_Shader.get(), "InDepth", global_color->DepthStencilAttachment->Handle, 3);

		DrawArray cmd =
		{
			.mode = DrawMode::Triangles,
			.first = 0,
			.count = 3
		};

		RenderCommands::BindVertexArrayObject(Context, Vao);
		RenderCommands::DrawArray(Context, cmd);



		Fracture::RenderCommands::ResetTextureUnits(Context, mSSAO_Shader.get());
		Fracture::RenderCommands::ReleaseRenderTarget(Context);
		Fracture::RenderCommands::UseProgram(Context, 0);
	}
	{
		Fracture::RenderCommands::SetRenderTarget(Context, global_SSAO);
		Fracture::RenderCommands::UseProgram(Context, mBlur_Shader->Handle);
		Fracture::RenderCommands::SetTexture(Context, mBlur_Shader.get(), "InSSAO", mSSAOrt->ColorAttachments[0]->Handle, 0);

		DrawArray cmd =
		{
			.mode = DrawMode::Triangles,
			.first = 0,
			.count = 3
		};

		RenderCommands::BindVertexArrayObject(Context, Vao);
		RenderCommands::DrawArray(Context, cmd);


		Fracture::RenderCommands::ResetTextureUnits(Context, mSSAO_Shader.get());
		Fracture::RenderCommands::UseProgram(Context, 0);
		Fracture::RenderCommands::ReleaseRenderTarget(Context);		
	}


}

float Fracture::SSAOPass::lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

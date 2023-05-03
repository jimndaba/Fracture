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
	GraphicsDevice::RenderSettings.SSAO_Resolution = glm::vec2(1280, 720);
	{
		randomFloats = std::uniform_real_distribution<float>(0.0, 1.0);
		for (unsigned int i = 0; i < 64; ++i)
		{
			glm::vec4 sample(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator), 0
			);
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			float scale = (float)i / 64.0;
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
	
	Fracture::RenderTargetCreationInfo info;
	{
		Fracture::TextureCreationInfo desc;
		desc.Width = GraphicsDevice::RenderSettings.SSAO_Resolution.x;
		desc.Height = GraphicsDevice::RenderSettings.SSAO_Resolution.y;
		desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
		desc.format = Fracture::TextureFormat::RGB;
		desc.internalFormat = Fracture::InternalFormat::RGB8;
		desc.formatType = Fracture::TextureFormatType::Int;
		desc.minFilter = TextureMinFilter::Linear;
		desc.magFilter = TextureMagFilter::Linear;
		desc.Name = "SSAO";
		info.ColorAttachments.push_back(desc);
	}
	{
		Fracture::TextureCreationInfo desc;
		desc.Width = GraphicsDevice::RenderSettings.SSAO_Resolution.x;
		desc.Height = GraphicsDevice::RenderSettings.SSAO_Resolution.y;
		desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
		desc.format = Fracture::TextureFormat::RGB;
		desc.internalFormat = Fracture::InternalFormat::RGB8;
		desc.formatType = Fracture::TextureFormatType::Int;
		desc.minFilter = TextureMinFilter::Linear;
		desc.magFilter = TextureMagFilter::Linear;
		desc.Name = "SSAO_Final";
		info.ColorAttachments.push_back(desc);
	}
	GraphicsDevice::Instance()->CreateGlobalRenderTarget("Global_SSAO", info);

	{
		mSSAO_Shader = AssetManager::GetShader("SSAO");
		mBlur_Shader = AssetManager::GetShader("SSAOBlur");
	}
	
}

void Fracture::SSAOPass::Execute()
{
}

float Fracture::SSAOPass::lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

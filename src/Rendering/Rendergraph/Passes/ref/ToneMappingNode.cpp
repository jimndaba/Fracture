#include "pch.h"
#include "ToneMappingNode.h"
#include "rendering/Device.h"
#include "rendering/Pipeline.h"

Fracture::ToneMappingNode::ToneMappingNode(DeviceContext& context, const  std::string& name,
	const std::vector<std::string>& inputs,
	const std::vector<std::string>& outputs) :
	RenderNode(context,name,inputs,outputs)
{
}

void Fracture::ToneMappingNode::Setup()
{
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\postprocessing\\ToneMapping.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\ToneMapping.frag";
		shader = Device::CreateShader(desc);
	}
	{
		BufferDescription desc;
		desc.Type = BufferType::UniformBuffer;
		desc.data = &Properties;
		desc.size = sizeof(ToneMappingParams);
		desc.usage = BufferUsage::Static;
		desc.Name = "TonemappingParams";
		mParams = Device::CreateBuffer(desc);
		Device::SetBufferIndexRange(mParams.get(), 10, 0);
	}
}

void Fracture::ToneMappingNode::Run(Scene& scene, CameraComponent* camera)
{
	OPTICK_CATEGORY("ToneMappingPass", Optick::Category::Rendering);
	const auto& prog = shader.get();
	const auto& read = Context.PostProcessReadTarget()->ColorAttachments[0]->RenderID;
	const auto& write = Context.PostProcessWriteTarget()->RenderID;

	Properties.ToneMapMode = (int)mode;
	Device::UpdateBufferData(mParams.get(), 0, sizeof(ToneMappingParams), &Properties);	
	Context.BindRenderTarget(write);
	Context.BindShader(shader->Program);
	Context.SetUniforms([prog, read]()
		{
			prog->SetTexture("InColorA", read, 0);
		});
	Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
	Context.DrawArrays(DrawMode::Triangle, 0, 3);
}

#include "pch.h"
#include "CompositionNode.h"
#include "PresentNode.h"
#include "rendering/Device.h"
#include "../Shader.h"
#include "rendering/Pipeline.h"
#include "rendering/Buffer.h"

Fracture::CompositionNode::CompositionNode(DeviceContext& context, const  std::string& name, const std::vector<std::string>& inputs, const std::vector<std::string>& outputs, MixMode mode) :
	RenderNode(context, name, inputs, outputs),Mode(mode)
{
}

void Fracture::CompositionNode::Setup()
{
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\postprocessing\\Composition.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\Composition.frag";
		shader = Device::CreateShader(desc);
	}
	{
		BufferDescription desc;
		desc.Type = BufferType::UniformBuffer;
		desc.data = &Properties;
		desc.size = sizeof(CompositionParams);
		desc.usage = BufferUsage::Static;
		desc.Name = "CompositionParams";
		mParams = Device::CreateBuffer(desc);
		Device::SetBufferIndexRange(mParams.get(), 8, 0);
	}
}

void Fracture::CompositionNode::Run(Scene& scene, CameraComponent* camera)
{
	OPTICK_CATEGORY("CompositionPass", Optick::Category::Rendering);

	int mode = (int)Mode;

	Device::UpdateBufferData(mParams.get(), 0, sizeof(CompositionParams), &Properties);

	const auto& prog = shader.get();
	const auto& colorA = Context.GetRenderReadTarget(InputResources[0].c_str())->ColorAttachments[0]->RenderID;
	const auto& colorB = Context.GetRenderReadTarget(InputResources[1].c_str())->ColorAttachments[0]->RenderID;
	const auto& write = Context.PostProcessWriteTarget()->RenderID;
	//const auto& write = Context.GetRenderWriteTarget(OutputResources[0].c_str())->RenderID;	

	Context.BindRenderTarget(write);
	
	Context.SetViewport(0, 0, Context.GetRenderReadTarget(InputResources[0].c_str())->ColorAttachments[0]->Description.Width, Context.GetRenderReadTarget(InputResources[0].c_str())->ColorAttachments[0]->Description.Height);

	Context.BindShader(shader->Program);
	Context.SetUniforms([prog, colorA, colorB, mode]()
		{
			
			prog->SetTexture("InColorA", colorA, 0);
			prog->SetTexture("InColorB", colorB, 1);
			prog->SetUniform("Mode", mode);
		});
	Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
	Context.DrawArrays(DrawMode::Triangle, 0, 3);	
	Context.BindRenderTarget(0);

	//Context.SetViewport(0, 0, Context.GetViewSize().x, Context.GetViewSize().y);
}

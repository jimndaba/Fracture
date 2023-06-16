#include "pch.h"
#include "PresentNode.h"
#include "rendering/Device.h"
#include "../Shader.h"
#include "rendering/Pipeline.h"
#include "core/Application.h"
#include "core/Components.h"
#include "scene/Scene.h"

Fracture::PresentNode::PresentNode(DeviceContext& context, const  std::string& name,
	const std::vector<std::string>& inputs,
	const std::vector<std::string>& outputs) :RenderNode(context, name, inputs, outputs)
{
}

void Fracture::PresentNode::Setup()
{
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\postprocessing\\Fullscreen.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\Fullscreen.frag";
		shader = Device::CreateShader(desc);
	}	
}

void Fracture::PresentNode::Run(Scene& scene, CameraComponent* camera)
{
	OPTICK_CATEGORY("PresentPass", Optick::Category::Rendering);
	const auto& prog = shader.get();
	const auto& read = Context.PostProcessReadTarget()->ColorAttachments[0]->RenderID;
	const auto& write = Context.PostProcessWriteTarget();
	const auto& siloute = Device::OutlineContext()->GetRenderTexture("OutlineTexture");
	const auto& outlineDepth = Device::OutlineContext()->GetRenderTexture("OutlineDepth");
	const auto& Debuglayer = Device::DebugContext()->GetRenderReadTarget("DebugTarget")->ColorAttachments[0].get();
	const auto& sceneDepth = Context.GetRenderReadTarget("BackBufferColor")->DepthStencil.get();

		
	Context.BindRenderTarget(write->RenderID);
	Context.SetViewport(0, 0, Context.PostProcessReadTarget()->ColorAttachments[0]->Description.Width, Context.PostProcessReadTarget()->ColorAttachments[0]->Description.Height);
	Context.ClearBuffers((uint32_t)GLClearBufferBit::Color | (uint32_t)GLClearBufferBit::Depth);
	Context.BindShader(shader->Program);



	Context.SetUniforms([=]()
		{ 
			prog->SetTexture("aDiffuse", read, 0);
		});

	if (siloute)
	{
		Context.SetUniforms([=]()
			{
				prog->SetUniform("Camera_Near_Far", glm::vec2(camera->Near, camera->Far));
				prog->SetTexture("aOutline", siloute->RenderID, 1);
				prog->SetTexture("aOutlineDepth", outlineDepth ->RenderID, 2);
				prog->SetTexture("aSceneDepth", sceneDepth->RenderID, 3);
				prog->SetTexture("aDebuglayer", Debuglayer->RenderID, 4);
				prog->SetUniform("_OutlineFlag", 1);
			});
	}
	else
	{
		Context.SetUniforms([=]()
			{			
				prog->SetUniform("_OutlineFlag", 0);
			});
	}

	Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
	Context.DrawArrays(DrawMode::Triangle, 0, 3);

	Context.SetUniforms([]()
		{
			glDisable(GL_BLEND);
		});
	
}

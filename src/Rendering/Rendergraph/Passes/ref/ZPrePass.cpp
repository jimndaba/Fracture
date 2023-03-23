#include "pch.h"
#include "ZPrePass.h"
#include "core/Application.h"
#include "rendering/Device.h"
#include "rendering/Buffer.h"
#include "rendering/Texture.h"
#include "assets/TextureLoader.h"
#include "rendering/StaticMesh.h"
#include "core/GameWindow.h"

Fracture::ZPrePassNode::ZPrePassNode(DeviceContext& context, const  std::string& name, const std::vector<std::string>& inputs, const std::vector<std::string>& outputs) :RenderNode(context, name, inputs, outputs)
{
}

void Fracture::ZPrePassNode::Setup()
{
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\color.vert";
		desc.FragmentPath = "Content\\shaders\\color.frag";
		shader = Device::CreateShader(desc);
	}
	
}

void Fracture::ZPrePassNode::Run(Scene& scene, CameraComponent* camera)
{
	OPTICK_CATEGORY("ZPrePass", Optick::Category::Rendering);
	Context.SetUniforms(
		[]()
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glDepthMask(true);
			glEnable(GL_CULL_FACE);
		}
	);


	Context.ClearColor(Color::CornflourBlue);
	Context.BindRenderTarget(Context.GetRenderWriteTarget("BackBufferColor")->RenderID);
	Context.SetViewport(0, 0, Context.GetRenderWriteTarget("BackBufferColor")->ColorAttachments[0]->Description.Width, Context.GetRenderWriteTarget("BackBufferColor")->ColorAttachments[0]->Description.Height);

	Context.ClearBuffers((uint32_t)GLClearBufferBit::Depth | (uint32_t)GLClearBufferBit::Color);	
	Context.BindShader(shader->Program);
	Context.SetColorMask(0, 0, 0, 0);
	for (auto& cmd : Context.GetOpaqueCommands())
	{
		Context.BindVertexArray(cmd->key.MeshIndex);
		Context.SetUniforms([=]() { shader->SetUniform("model", cmd->Transform); });
		DrawCommand::DrawElementArraysBaseVertex(cmd.get(), &Context);
	}
	Context.SetColorMask(1, 1, 1, 1);
	Context.UnbindRenderTarget();
	Context.SetUniforms(
		[]()
		{
			glDisable(GL_DEPTH_TEST);
		}
	);
}

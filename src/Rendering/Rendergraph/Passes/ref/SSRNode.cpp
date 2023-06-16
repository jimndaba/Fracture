#include "pch.h"
#include "SSRNode.h"
#include "rendering/Device.h"
#include "rendering/Pipeline.h"
#include "assets/TextureLoader.h"
#include "rendering/RenderTarget.h"
#include "core/Application.h"
#include "core/GameWindow.h"

Fracture::SSRNode::SSRNode(DeviceContext& context, const  std::string& name, const std::vector<std::string>& inputs, const std::vector<std::string>& outputs) :
	RenderNode(context, name, inputs, outputs)
{
}

Fracture::SSRNode::~SSRNode()
{
}

void Fracture::SSRNode::Setup()
{
	//Resolution = glm::vec2(Context.GetViewSize().x, Context.GetViewSize().y);
	Resolution = glm::vec2(1920 , 1080);
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\postprocessing\\SSR.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\SSR.frag";
		shader = Device::CreateShader(desc);
	}

	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\postprocessing\\Composition.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\Composition.frag";
		combineshader = Device::CreateShader(desc);
	}
	{
		BufferDescription desc;
		desc.Type = BufferType::UniformBuffer;
		desc.data = &Properties;
		desc.size = sizeof(SSRParams);
		desc.usage = BufferUsage::Static;
		desc.Name = "SSR PArams Buffer";
		mParamsBuffer = Device::CreateBuffer(desc);
		Device::SetBufferIndexRange(mParamsBuffer.get(), 12, 0);
	}
	{
		TextureDescription desc;
		desc.Name = "SSRTexture";
		desc.ID = UUID();
		desc.NoChannels = 1;
		desc.MipLevels = 1;
		desc.GenMinMaps = false;
		desc.formatType = FormatType::Float;
		desc.format = Format::RGBA;
		desc.internalFormat = InternalFormat::RGBA16F;
		desc.TextureTarget = TextureTarget::Texture2D;
		desc.minFilter = TextureMinFilter::Near;
		desc.magFilter = TextureMagFilter::Near;
		desc.Wrap = TextureWrap::ClampToEdge;
		desc.Width = 1920;
		desc.Height = 1080;


		TextureLoader loader;
		mSSRTexture = loader.Load(desc);
		Device::CreateTexture(mSSRTexture.get());
	}
	{
		ResourceDescription res_desc;
		res_desc.Name = "SSR_Out";
		res_desc.TextureTarget = TextureTarget::Texture2D;
		res_desc.TextureAttachmentTarget = AttachmentTarget::Color;
		res_desc.ColorBufferInternalformats = { InternalFormat::RGBA16F };
		res_desc.Width = Resolution.x;
		res_desc.Height = Resolution.y;
		res_desc.IsResizable = false;
		res_desc.magFilter = TextureMagFilter::Linear;
		res_desc.minFilter = TextureMinFilter::Linear;
		res_desc.Type = ResourceType::RenderTarget;
		Context.AddRenderResource(res_desc);
	}
}

void Fracture::SSRNode::Run(Scene& scene, CameraComponent* camera)
{

	
	const auto& prog = shader.get();
	const auto& props = Properties;

	const auto& inDepth = Context.GetRenderReadTarget("BackBufferColor")->DepthStencil->RenderID;
	const auto& inColor = Context.GetRenderReadTarget("BackBufferColor")->ColorAttachments[0]->RenderID;
	const auto& inMetalic = Context.GetRenderReadTarget("BackBufferColor")->ColorAttachments[1]->RenderID;

	float AspectRatio = Resolution.x / Resolution.y;
	float TanHalfFOV = tanf(glm::radians(90.0f / 2.0f));

	Device::UpdateBufferData(mParamsBuffer.get(), 0, sizeof(SSRParams), &Properties);


	Context.SetViewport(0, 0, Resolution.x, Resolution.y);
	{
		Context.BindRenderTarget(Context.GetRenderWriteTarget("SSR_Out")->RenderID);
		Context.ClearColor(Color::Black);
		Context.ClearBuffers((uint32_t)GLClearBufferBit::Color);
		Context.BindShader(shader->Program);
		Context.SetUniforms([prog, props, inDepth, inMetalic, inColor]()
			{
				prog->SetTexture("InDepth", inDepth, 0);
				prog->SetTexture("InMetalic", inMetalic, 1);
				prog->SetTexture("InColor", inColor, 2);
			});

		Context.SetUniforms([=]() {
			glNamedFramebufferTexture(Context.GetRenderWriteTarget("SSR_Out")->RenderID, GL_COLOR_ATTACHMENT0, mSSRTexture->RenderID, 0);
			});

		Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
		Context.DrawArrays(DrawMode::Triangle, 0, 3);
		Context.UnbindRenderTarget();
	}
	Context.SetViewport(0, 0, Context.GetRenderReadTarget("BackBufferColor")->ColorAttachments[0]->Description.Width, Context.GetRenderReadTarget("BackBufferColor")->ColorAttachments[0]->Description.Height);

	{

		const auto& prog = combineshader.get();

		const auto& colorA = mSSRTexture->RenderID;
		const auto& colorB = Context.PostProcessReadTarget();

		const auto& write = Context.PostProcessWriteTarget();
		
		
		Context.BindRenderTarget(write->RenderID);
		Context.BindShader(combineshader->Program);
		Context.SetUniforms([=]()
			{

				prog->SetTexture("InColorA", colorA, 0);
				prog->SetTexture("InColorB", colorB->ColorAttachments[0]->RenderID, 1);
				prog->SetUniform("Mode", 0);
			});
		Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
		Context.DrawArrays(DrawMode::Triangle, 0, 3);
		Context.BindRenderTarget(0);

	}
}

void Fracture::SSRNode::ReloadShader()
{
	std::cout << "Reloading SSR shader\n";
	Device::ReloadShader(shader.get());
}

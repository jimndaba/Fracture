#include "pch.h"
#include "PickingPass.h"
#include "../../RenderTarget.h"
#include "../../DrawCommands.h"
#include "../../RenderBucket.h"
#include "../../Material.h"
#include "Rendering/Shader.h"
#include "Entity/ILight.h"
#include "Components/ComponentManager.h"
#include "Components/TransformComponent.h"
#include "Rendering/Texture2D.h"
#include "../../Renderer.h"
#include "../InputSocket.h"
#include "../OutputSocket.h"
#include "Components/ICamera.h"
#include "Assets/AssetManager.h"

std::shared_ptr<Fracture::RenderTarget> Fracture::PickingPass::m_renderTarget;

Fracture::PickingPass::PickingPass(const std::string& Name, const Renderer& renderer) :
	RenderQueueNode(Name),
	m_pixelInfo(std::make_shared<PixelInfo>()),
	m_shader(AssetManager::GetShader("PickingShader"))
{
	RenderTargetParams params_rt = RenderTargetParams();
	params_rt.name = "PickingPass";
	params_rt.width = renderer.Width();
	params_rt.height = renderer.Height();
	params_rt.texturetarget = AttachmentTarget::Texture2D;
	params_rt.nrColorAttachments = 1;
	params_rt.depthAndStencil = true;
	params_rt.type = FormatType::Int;


	TextureParams params_textures = TextureParams();
	params_textures.format = TextureFormat::RGBA;
	params_textures.internalformat = InternalFormat::RGBA;
	params_textures.formatType = FormatType::Int;
	params_textures.width = renderer.Width();
	params_textures.height = renderer.Height();
	params_textures.MinFilter = TextureMinFilter::Linear;
	params_textures.MagFilter = TextureMagFilter::Linear;
	params_textures.wrap = glWrap::ClampToEdge;
	params_textures.GenMips = false;

	m_renderTarget = RenderTarget::CreateRenderTarget(params_rt,params_textures);



}

Fracture::PickingPass::~PickingPass()
{
}

void Fracture::PickingPass::execute(Renderer& renderer)
{
	m_renderTarget->bind();
	uint32_t mWidth = m_renderTarget->GetColorTexture(0)->GetWidth();
	uint32_t mHeight = m_renderTarget->GetColorTexture(0)->GetHeight();
	

	glDisable(GL_DITHER);
	int clearValue = 1;
	Renderer::SubmitFunc({ [mWidth,mHeight]() {RenderCommand::SetViewport(mWidth,mHeight); } });
	
	Renderer::SubmitFunc({ []() {RenderCommand::ClearColor(glm::vec4(0.1f)); } });
	Renderer::SubmitFunc({ []() {RenderCommand::ClearBuffers(); } });
	
	glClearTexImage(m_renderTarget->GetColorTexture(0)->GetTextureID(), 0, GL_RGBA, GL_INT, &clearValue);

	const std::vector<DrawCommand>& forwardRenderCommands = renderer.GetBucket("Opaque")->GetCommands();	
	for (const auto& command : forwardRenderCommands)
	{
		m_shader->use();
		int r = ((uint32_t)command.EntityID & 0x000000FF) >> 0;
		int g = ((uint32_t)command.EntityID & 0x0000FF00) >> 8;
		int b = ((uint32_t)command.EntityID & 0x00FF0000) >> 16;

		m_shader->setVec4("pickingColorID", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f,1.0f));
		m_shader->setMat4("view", renderer.ActiveCamera()->getViewMatrix());
		m_shader->setMat4("projection", renderer.ActiveCamera()->getProjectionMatrix());
		m_shader->setMat4("model", command.ModelTransform);

		RenderCommand::BindVertexArray(command.VertexBuffer);
		RenderCommand::DrawElementsBase(DrawMode::Triangles, command.IndexCount, (void*)(sizeof(uint32_t) * command.BaseIndex), command.BaseVertex);
	}

	const std::vector<DrawCommand>& alphaRenderCommands = renderer.GetBucket("Transparent")->GetCommands();
	for (const auto& command : alphaRenderCommands)
	{	
		m_shader->use();
		int r = ((uint32_t)command.EntityID & 0x000000FF) >> 0;
		int g = ((uint32_t)command.EntityID & 0x0000FF00) >> 8;
		int b = ((uint32_t)command.EntityID & 0x00FF0000) >> 16;
		
		m_shader->setVec4("pickingColorID", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f));
		m_shader->setMat4("view", renderer.ActiveCamera()->getViewMatrix());
		m_shader->setMat4("projection", renderer.ActiveCamera()->getProjectionMatrix());
		m_shader->setMat4("model", command.ModelTransform);

		RenderCommand::BindVertexArray(command.VertexBuffer);
		RenderCommand::DrawElementsBase(DrawMode::Triangles, command.IndexCount, (void*)(sizeof(uint32_t) * command.BaseIndex), command.BaseVertex);
	}
	

	glEnable(GL_DITHER);
	m_renderTarget->Unbind();
}


void Fracture::PickingPass::Resize(int screenWidth, int screenHeight)
{
	m_renderTarget->Resize(screenWidth,screenHeight);
	SCREEN_HEIGHT = screenHeight;
	SCREEN_WIDTH = screenWidth;
}

uint32_t Fracture::PickingPass::GetPixelInfo(unsigned int x, unsigned int y)
{
	m_renderTarget->bind();
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	FRACTURE_INFO("width: {}", m_renderTarget->Width);
	FRACTURE_INFO("height: {}", m_renderTarget->Height);


	unsigned char id[4];
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &id);

	uint32_t pickedID =
		id[0] +
		id[1] * 256 +
		id[2] * 256 * 256;

	if (pickedID == 0x00ffffff) { // Full white, must be the background !
		return -1;
	}

	glReadBuffer(GL_NONE);
	m_renderTarget->Unbind();

	return pickedID;
}

nlohmann::json Fracture::PickingPass::Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor)
{
	return json();
}

#include "PickingPass.h"
#include "../../RenderTarget.h"
#include "../../DrawCommand.h"
#include "../../RenderBucket.h"
#include "../../Material.h"
#include "Rendering/Shader.h"
#include "Entity/ILight.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "../../Renderer.h"
#include "Component/ICamera.h"

std::shared_ptr<Fracture::RenderTarget> Fracture::PickingPass::m_renderTarget;

Fracture::PickingPass::PickingPass(const std::string& Name,const int& width,const int& height, const std::shared_ptr<RenderBucket>& opaque):
	RenderQueueNode(Name,opaque),
	SCREEN_WIDTH(width),
	SCREEN_HEIGHT(height),
	m_pixelInfo(std::make_shared<PixelInfo>())
{
	m_renderTarget = RenderTarget::CreateRenderTarget("PickingPass", SCREEN_WIDTH, SCREEN_HEIGHT, AttachmentTarget::Texture2D, FormatType::Float, 1, false);

}

Fracture::PickingPass::~PickingPass()
{
}

void Fracture::PickingPass::execute(Renderer& renderer)
{
	//renderer.setViewport(renderer.Width(), renderer.Height());
	glDisable(GL_DITHER);
	int clearValue = -1;
	glClearTexImage(m_renderTarget->GetColorTexture(0)->GetTextureID(), 0, GL_RGBA, GL_UNSIGNED_BYTE, &clearValue);

	const auto& bucket = GetBucket();
	
	for (const auto& command : bucket->getForwardRenderCommands())
	{
		std::shared_ptr<Material> material = AssetManager::getMaterial("PickingMaterial");
		material->use();
		int r = ((int)command.ID & 0x000000FF) >> 0;
		int g = ((int)command.ID & 0x0000FF00) >> 8;
		int b = ((int)command.ID & 0x00FF0000) >> 16;

		//material->getShader()->setInt("gObjectIndex",command.ID);
		material->getShader()->setVec4("pickingColorID", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f));
		//material->getShader()->setMat4("view", renderer.ActiveCamera()->getViewMatrix());
		//material->getShader()->setMat4("projection", renderer.ActiveCamera()->getProjectionMatrix());
		//material->getShader()->setMat4("model", command.Transform);

		renderer.Submit(command,material->getShader().get());
		
		//glBindVertexArray(command.VAO);
		//glDrawElements(GL_TRIANGLES, command.indiceSize, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
	}
	for (const auto& command : bucket->getAlphaRenderCommands())
	{
		std::shared_ptr<Material> material = AssetManager::getMaterial("PickingMaterial");
		material->use();
		int r = ((int)command.ID & 0x000000FF) >> 0;
		int g = ((int)command.ID & 0x0000FF00) >> 8;
		int b = ((int)command.ID & 0x00FF0000) >> 16;

		//material->getShader()->setInt("gObjectIndex",command.ID);
		material->getShader()->setVec4("pickingColorID", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f));

		renderer.Submit(command, material->getShader().get());
		//material->getShader()->setMat4("view", renderer.ActiveCamera()->getViewMatrix());
		//material->getShader()->setMat4("projection", renderer.ActiveCamera()->getProjectionMatrix());
		//material->getShader()->setMat4("model", command.Transform);
		//
		//glBindVertexArray(command.VAO);
		//glDrawElements(GL_TRIANGLES, command.indiceSize, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);
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

unsigned int Fracture::PickingPass::GetPixelInfo(unsigned int x, unsigned int y)
{
	m_renderTarget->bind();
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	FRACTURE_INFO("width: {}", m_renderTarget->Width);
	FRACTURE_INFO("height: {}", m_renderTarget->Height);


	unsigned char id[4];
	glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &id);

	unsigned int pickedID =
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

#include "PickingPass.h"
#include "../../RenderTarget.h"
#include "../../DrawCommand.h"
#include "../../RenderBucket.h"
#include "../../RenderBatch.h"
#include "../../Material.h"
#include "Rendering/Shader.h"
#include "Component/ILight.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "../../Renderer.h"
#include "Component/ICamera.h"

std::shared_ptr<Fracture::RenderTarget> Fracture::PickingPass::m_renderTarget;

Fracture::PickingPass::PickingPass(std::string Name, int width, int height, RenderBucket* opaque):RenderQueueNode(Name),SCREEN_WIDTH(width),SCREEN_HEIGHT(height)
{
	m_renderTarget = std::shared_ptr<RenderTarget>(new RenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT, GL_FLOAT, 1,false));
	AcceptBucket(opaque);
	m_pixelInfo = std::make_shared<PixelInfo>();
}

Fracture::PickingPass::~PickingPass()
{
}

void Fracture::PickingPass::execute(Renderer& renderer)
{
	renderer.setViewport(renderer.Width(), renderer.Height());
	glDisable(GL_DITHER);
	int clearValue = -1;
	glClearTexImage(m_renderTarget->GetColorTexture(0)->id, 0, GL_RGBA, GL_UNSIGNED_BYTE, &clearValue);

	for (auto& bucket : m_buckets)
	{		
		for (const auto& command : bucket->getForwardRenderCommands())
		{			
			std::shared_ptr<Material> material = AssetManager::getMaterial("PickingMaterial");
			material->use();
			int r = ((int)command.ID & 0x000000FF) >> 0;
			int g = ((int)command.ID & 0x0000FF00) >> 8;
			int b = ((int)command.ID & 0x00FF0000) >> 16;

			//material->getShader()->setInt("gObjectIndex",command.ID);
			material->getShader()->setVec4("pickingColorID", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f));
			material->getShader()->setMat4("view", renderer.ActiveCamera()->getViewMatrix());
			material->getShader()->setMat4("projection", renderer.ActiveCamera()->getProjectionMatrix());
			if (ComponentManager::HasComponent<TransformComponent>(command.ID))
			{
				material->getShader()->setMat4("model", ComponentManager::GetComponent<TransformComponent>(command.ID)->GetWorldTransform());
			}
			glBindVertexArray(command.VAO);
			glDrawElements(GL_TRIANGLES, command.indiceSize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
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
			material->getShader()->setMat4("view", renderer.ActiveCamera()->getViewMatrix());
			material->getShader()->setMat4("projection", renderer.ActiveCamera()->getProjectionMatrix());
			if (ComponentManager::HasComponent<TransformComponent>(command.ID))
			{
				material->getShader()->setMat4("model", ComponentManager::GetComponent<TransformComponent>(command.ID)->GetWorldTransform());
			}
			glBindVertexArray(command.VAO);
			glDrawElements(GL_TRIANGLES, command.indiceSize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
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

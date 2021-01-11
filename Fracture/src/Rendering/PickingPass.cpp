#include "PickingPass.h"
#include "RenderTarget.h"
#include "RenderCommand.h"
#include "RenderBucket.h"
#include "Material.h"
#include "Shader.h"
#include "Component/ILight.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Renderer.h"
#include "Component/ICamera.h"


Fracture::PickingPass::PickingPass(int screenWidth, int screenHeight):SCREEN_WIDTH(screenWidth),SCREEN_HEIGHT(screenHeight)
{
	m_renderTarget = std::shared_ptr<RenderTarget>(new RenderTarget(screenWidth, screenHeight, GL_FLOAT, 1, true));
	m_pixelInfo = std::make_shared<PixelInfo>();
}

Fracture::PickingPass::~PickingPass()
{
}

void Fracture::PickingPass::Begin()
{
	m_renderTarget->bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f,1.0f);
	glDisable(GL_DITHER);

	int clearValue = -1;
	glClearTexImage(m_renderTarget->GetColorTexture(0)->id, 0, GL_RGBA, GL_UNSIGNED_BYTE, &clearValue);
}

void Fracture::PickingPass::End()
{
	m_renderTarget->Unbind();
	glEnable(GL_DITHER);
}

void Fracture::PickingPass::Render(std::shared_ptr<ICamera> camera,std::shared_ptr<Material> material, RenderBucket& bucket)
{
	for (const auto& command : bucket.getCommands())
	{
		material->getShader()->use();
		//m_pickingEffect.SetObjectIndex(i);
		// Convert "i", the integer mesh ID, into an RGB color
		int r = ((int)command.ID & 0x000000FF) >> 0;
		int g = ((int)command.ID & 0x0000FF00) >> 8;
		int b = ((int)command.ID & 0x00FF0000) >> 16;

		//material->getShader()->setInt("gObjectIndex",command.ID);
		material->getShader()->setVec4("pickingColorID", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f));
		material->getShader()->setMat4("view", camera->getViewMatrix());
		material->getShader()->setMat4("projection", camera->getProjectionMatrix());
		material->getShader()->setMat4("model", ComponentManager::GetComponent<TransformComponent>(command.ID)->GetWorldTransform());
		glBindVertexArray(command.VAO);
		glDrawElements(GL_TRIANGLES, command.indiceSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
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

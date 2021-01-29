#include "DepthNode.h"

#include "Rendering/RenderTarget.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderBucket.h"
#include "Rendering/RenderBatch.h"
#include "AssetManager/AssetManager.h"
#include "Component/ICamera.h"
#include "Rendering/Material.h"
#include "Profiling/Profiler.h"

Fracture::DepthNode::DepthNode(std::string name, int width, int height, RenderBucket* opaque):RenderQueueNode(name)
{
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("outputDepthMap");

	outputDepthMap = std::make_shared<RenderTarget>(width, height, GL_FLOAT, 1, true);

	m_shader = AssetManager::getShader("DepthPass");

	AcceptBucket(opaque);

	AddOutputSocket(m_output);
	AddOutputResource(m_output, outputDepthMap);
}

void Fracture::DepthNode::execute(Renderer& renderer)
{
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
	ProfilerTimer timer("Depth Pre-Pass");
	resources["outputDepthMap"]->bind();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	renderer.clear();
	m_shader->use();
	m_shader->setFloat("nearPlane", renderer.ActiveCamera()->Near());
	m_shader->setFloat("farPlane", renderer.ActiveCamera()->Far());
    for (auto& bucket : m_buckets)
    {
		std::vector<DrawCommand> forwardRenderCommands = bucket->getForwardRenderCommands();
		std::vector<DrawCommand> alphaRenderCommands = bucket->getAlphaRenderCommands();

        for (unsigned int i = 0; i < forwardRenderCommands.size(); ++i)
        {
            DrawCommand command = forwardRenderCommands[i];
            renderer.Submit(command, m_shader.get());
        }

        for (unsigned int i = 0; i < alphaRenderCommands.size(); ++i)
        {
            DrawCommand command = alphaRenderCommands[i];
			renderer.Submit(command, m_shader.get());
        }
    }
	resources["outputDepthMap"]->Unbind();
}

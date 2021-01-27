#include "DepthNode.h"

#include "Rendering/RenderTarget.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderBucket.h"
#include "Rendering/RenderBatch.h"
#include "AssetManager/AssetManager.h"
#include "Component/ICamera.h"
#include "Rendering/Material.h"

Fracture::DepthNode::DepthNode(std::string name, int width, int height, RenderBucket* opaque, RenderBucket* transparent):RenderQueueNode(name)
{
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("outputDepthMap");

	outputDepthMap = std::make_shared<RenderTarget>(width, height, GL_FLOAT, 1, true);

	m_shader = AssetManager::getShader("DepthPass");

	AcceptBucket(opaque);
	AcceptBucket(transparent);
	
	AddOutputSocket(m_output);
	AddOutputResource(m_output, outputDepthMap);
}

void Fracture::DepthNode::execute(Renderer& renderer)
{
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))
	resources["outputDepthMap"]->bind();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	renderer.clear();
	m_shader->use();
	m_shader->setFloat("nearPlane", renderer.ActiveCamera()->Near());
	m_shader->setFloat("farPlane", renderer.ActiveCamera()->Far());
    for (auto& bucket : m_buckets)
    {
        for (const auto& batch : bucket->getRenderBatches())
        {           
            for (auto command : batch.second->m_commnads)
            {
                renderer.Submit(command,m_shader.get());
            }
        }       
    }
	resources["outputDepthMap"]->Unbind();
}

#include "DepthNode.h"

#include "Rendering/RenderTarget.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Rendering/Renderer.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderBucket.h"
#include "Rendering/RenderBatch.h"
#include "AssetManager/AssetManager.h"
#include "Component/ICamera.h"
#include "Rendering/Material.h"
#include "Profiling/Profiler.h"

Fracture::DepthNode::DepthNode(const std::string& name, const int& width, const int& height, RenderBucket* opaque) :
	RenderQueueNode(name),
	m_shader(AssetManager::getShader("DepthPass"))
{
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("outputDepthMap");

	//outputDepthMap = std::make_shared<RenderTarget>("DepthPass_out",width, height, TextureTarget::Texture2D,GL_FLOAT, 1, true);

	outputDepthMap = RenderTarget::CreateRenderTarget("DepthPass_out", width, height, glAttachmentTarget::Texture2D,FormatType::Float, 1, true);


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
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	renderer.clear();
	m_shader->use();
	m_shader->setFloat("nearPlane", renderer.ActiveCamera()->Near());
	m_shader->setFloat("farPlane", renderer.ActiveCamera()->Far());
    for (const auto& bucket : m_buckets)
    {
		const auto& forwardRenderCommands = bucket->getForwardRenderCommands();
		const auto& alphaRenderCommands = bucket->getAlphaRenderCommands();

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

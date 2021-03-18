#include "SSRNode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Renderer.h"
#include "Component/ICamera.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Profiling/Profiler.h"
#include <random>

Fracture::SSRNode::SSRNode(const std::string& name, const int& width, const int& height) :
    FullScreenNode(name),
    m_shader(AssetManager::getShader("SSRPASS"))
{
    std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("DepthTexture");
    std::shared_ptr<InputSocket> m_AlbedoInput = std::make_shared<InputSocket>("AlbedoTexture");
    std::shared_ptr<InputSocket> m_MaskInput = std::make_shared<InputSocket>("MaskTexture");
    std::shared_ptr<InputSocket> m_NormalInput = std::make_shared<InputSocket>("NormalTexture");
    std::shared_ptr<InputSocket> m_PositionlInput = std::make_shared<InputSocket>("PositionTexture");

    std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("SSROutput");

    ssr = RenderTarget::CreateRenderTarget("SSRpass", width, height, AttachmentTarget::Texture2D, FormatType::Float, 1, false);
    
    AddResource("ssrtexture", ssr);

    //Sockets
    AddInputSocket(m_Input);
    AddInputSocket(m_AlbedoInput);
    AddInputSocket(m_MaskInput);
    AddInputSocket(m_NormalInput);
    AddInputSocket(m_PositionlInput);

    AddOutputSocket(m_output);
    AddOutputResource(m_output, ssr);

}

void Fracture::SSRNode::execute(Renderer& renderer)
{
    resources["ssrtexture"]->bind();
    m_shader->use();
  
    m_shader->setMat4("projection", renderer.ActiveCamera()->getProjectionMatrix());
    m_shader->setMat4("invprojection", glm::inverse(renderer.ActiveCamera()->getProjectionMatrix()));
    m_shader->setMat4("view", renderer.ActiveCamera()->getViewMatrix());
    m_shader->setMat4("invView", glm::inverse(renderer.ActiveCamera()->getViewMatrix()));
    m_shader->setFloat("nearPlane", renderer.ActiveCamera()->Near());
    m_shader->setFloat("farPlane", renderer.ActiveCamera()->Far());

    m_shader->setTexture("depthTexture", resources["DepthTexture"]->GetColorTexture(0).get(), 0);
    m_shader->setTexture("albedoTexture", resources["AlbedoTexture"]->GetColorTexture(0).get(), 1);
    m_shader->setTexture("maskTexture", resources["MaskTexture"]->GetColorTexture(0).get(), 2);
    m_shader->setTexture("normalTexture", resources["NormalTexture"]->GetColorTexture(0).get(), 3);
    m_shader->setTexture("positionTexture", resources["PositionTexture"]->GetColorTexture(0).get(),4);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    m_shader->unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
    resources["ssrtexture"]->Unbind();
}

nlohmann::json Fracture::SSRNode::Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor)
{
    return json();
}

#include "SSAONode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Renderer.h"
#include "Component/ICamera.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Profiling/Profiler.h"
#include <random>


Fracture::SSAONode::SSAONode(const std::string& name,const int& width,const int& height):
	FullScreenNode(name),
	m_shader(AssetManager::getShader("SSAOPASS")),
	m_blurshader(AssetManager::getShader("SSAOBLUR"))
{
	std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("DepthTexture");
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("SSAOOutput");

	ssao = RenderTarget::CreateRenderTarget("ssaopass",width,height,AttachmentTarget::Texture2D,FormatType::Float,1,false);

	outputTexture = RenderTarget::CreateRenderTarget("SSAO_out",width, height, AttachmentTarget::Texture2D, FormatType::Float, 1, false);

	AddResource("ssaopass",ssao);
	
	//Sockets
	AddInputSocket(m_Input);
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, resource);
	AddOutputResource(m_output, outputTexture);
	
	//noise TODO Move to Noise Texture Node
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;	
	std::vector<glm::vec3> ssaoNoise;

	unsigned int noise_res = 16;

	for (unsigned int i = 0; i < noise_res; i++)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0, 
			randomFloats(generator) * 2.0 - 1.0, 
			0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	
	m_noiseTexture = Texture2D::CreateTexture(&ssaoNoise[0], InternalFormat::RGBA16,TextureFormat::RGB, 4, 4, glWrap::ClampToEdge, FormatType::Float);

	//m_noiseTexture = std::shared_ptr<Texture>(new Texture("ssao_noise", 4, 4, TextureType::Diffuse, &ssaoNoise[0]));
	
}

void Fracture::SSAONode::execute(Renderer& renderer)
{
	ProfilerTimer timer("SSAO Pass");
	
	resources["ssaopass"]->bind();
	m_shader->use();
	float TanHalfFOV = tanf(renderer.ActiveCamera()->GetFOV()/ 2.0f);
	m_shader->setFloat("gAspectRatio", renderer.ActiveCamera()->GetApectRatio());
	m_shader->setFloat("gTanHalfFOV", TanHalfFOV);
	m_shader->setFloat("total_strength", total_strength);
	m_shader->setFloat("area", area);
	m_shader->setFloat("falloff", falloff);
	m_shader->setFloat("radius", radius);
	m_shader->setFloat("nearPlane", renderer.ActiveCamera()->Near());
	m_shader->setFloat("farPlane", renderer.ActiveCamera()->Far());
	m_shader->setMat4("invprojection", glm::inverse(renderer.ActiveCamera()->getProjectionMatrix()));
	m_shader->setMat4("projection", renderer.ActiveCamera()->getProjectionMatrix());
	m_shader->setTexture("depthTexture", resources["DepthTexture"]->GetColorTexture(0).get(), 0);
	m_shader->setTexture("texNoise", m_noiseTexture.get(), 1);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_shader->unbind();
	resources["ssaopass"]->bind();

	resources["SSAOOutput"]->bind();
	m_blurshader->use();
	m_shader->setTexture("ssaoInput", resources["ssaopass"]->GetColorTexture(0).get(), 0);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_blurshader->unbind();
	resources["SSAOOutput"]->Unbind();

}

nlohmann::json Fracture::SSAONode::Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor)
{
	return visitor->visitSSAONode(*this);
}

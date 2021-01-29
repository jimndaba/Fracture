#include "SSAONode.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/Renderer.h"
#include "Component/ICamera.h"
#include "Rendering/Texture.h"
#include "Profiling/Profiler.h"
#include <random>


Fracture::SSAONode::SSAONode(std::string name, int width, int height):FullScreenNode(name)
{
	std::shared_ptr<InputSocket> m_Input = std::make_shared<InputSocket>("DepthTexture");
	std::shared_ptr<OutputSocket> m_output = std::make_shared<OutputSocket>("SSAOOutput");

	outputTexture = std::make_shared<RenderTarget>(width, height, GL_FLOAT, 1, false);

	m_shader = AssetManager::getShader("SSAOPASS");

	//Sockets
	AddInputSocket(m_Input);
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, resource);
	//AddInputResource(m_EnvironmentInput, resource);
	AddOutputResource(m_output, outputTexture);
	
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;
	//noise
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
	
	m_noiseTexture = std::shared_ptr<Texture>(new Texture("ssao_noise", 4, 4, TextureType::Diffuse, &ssaoNoise[0]));
	
}

void Fracture::SSAONode::execute(Renderer& renderer)
{
	ProfilerTimer timer("SSAO Pass");
	resources["SSAOOutput"]->bind();
	glBindVertexArray(quadVAO);
	m_shader->use();
	float TanHalfFOV = tanf(renderer.ActiveCamera()->GetFOV()/ 2.0f);
	m_shader->setFloat("gAspectRatio", renderer.ActiveCamera()->GetApectRatio());
	m_shader->setFloat("gTanHalfFOV", TanHalfFOV);


	m_shader->setFloat("total_strength", total_strength);
	m_shader->setFloat("base", base);
	m_shader->setFloat("area", area);
	m_shader->setFloat("falloff", falloff);
	m_shader->setFloat("bias", bias);
	m_shader->setFloat("radius", radius);

	m_shader->setFloat("nearPlane", renderer.ActiveCamera()->Near());
	m_shader->setFloat("farPlane", renderer.ActiveCamera()->Far());
	m_shader->setMat4("projection", glm::inverse(renderer.ActiveCamera()->getProjectionMatrix()));
	m_shader->setTexture("depthTexture", resources["DepthTexture"]->GetColorTexture(0).get(), 0);
	m_shader->setTexture("texNoise", m_noiseTexture.get(), 1);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_shader->unbind();
	resources["SSAOOutput"]->Unbind();

}

#include "RenderQueueNode.h"
#include "Rendering/Shader.h"
#include "Rendering/Material.h"
#include "Rendering/OpenGL/TextureCubeMap.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderBucket.h"
#include "AssetManager/AssetManager.h"
#include "Profiling/Profiler.h"
#include "GLAD/glad.h"

Fracture::RenderQueueNode::RenderQueueNode(std::string name, const std::shared_ptr<RenderBucket>& bucket):FrameNode(name),m_bucket(bucket)
{
}

void Fracture::RenderQueueNode::execute(Renderer& renderer)
{
}

void Fracture::RenderQueueNode::render(Renderer& renderer)
{   
    ProfilerTimer timer("RenderQueue Render");    
    m_bucket->sortForward();
    m_bucket->sortAlpha();
    //1. Geometry buffers
    renderer.setViewport(renderer.Width(),renderer.Height());
    std::vector<DrawCommand> forwardRenderCommands = m_bucket->getForwardRenderCommands();
    std::vector<DrawCommand> alphaRenderCommands = m_bucket->getAlphaRenderCommands();


    for (unsigned int i = 0; i < forwardRenderCommands.size(); ++i)
    {
        DrawCommand command = forwardRenderCommands[i];
        NumberBatches += 1;
        Material* material = command.material;
        material->use();
        material->getShader()->setTexture("ambientOcclusion", resources["SSAOMap"]->GetColorTexture(0).get(), 8);
        renderer.SetupLighting(material);
        const auto& uniforms = material->GetUniforms();
        for (const auto& u : *uniforms)
        {
            ProfilerTimer timer("RQ WriteUNiforms");
            WriteUniformData(*material->getShader(), u.first, u.second);
        }

        const auto& uniformsSamplers = material->GetSamplerUniforms();
        for (const auto& sample : *uniformsSamplers)
        {
            ProfilerTimer timer("RQ WriteSamples");
            WriteUniformSampler(*material->getShader(), sample.first, sample.second);
        }

        renderer.Submit(command);
    }


    if (alphaRenderCommands.size() > 0)
    {
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        for (unsigned int i = 0; i < alphaRenderCommands.size(); ++i)
        {
            DrawCommand command = alphaRenderCommands[i];
            NumberBatches += 1;
            Material* material = command.material;
            material->use();
            material->getShader()->setTexture("ambientOcclusion", resources["SSAOMap"]->GetColorTexture(0).get(), 8);
            material->getShader()->setInt("TransparencyFlag", command.material->IsTransparent());
            renderer.SetupLighting(material);

            const auto& uniforms = material->GetUniforms();
            for (const auto& u : *uniforms)
            {
                ProfilerTimer timer("RQ WriteUNiforms");
                WriteUniformData(*material->getShader(), u.first, u.second);
            }

            const auto& uniformsSamplers = material->GetSamplerUniforms();
            for (const auto& sample : *uniformsSamplers)
            {
                ProfilerTimer timer("RQ WriteSamples");
                WriteUniformSampler(*material->getShader(), sample.first, sample.second);
            }

            renderer.Submit(command);
        }
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
    }
}

std::shared_ptr<Fracture::RenderBucket> Fracture::RenderQueueNode::GetBucket()
{
	return m_bucket;
}

void Fracture::RenderQueueNode::WriteUniformData(Shader shader, std::string name, UniformValue value)
{
    switch (value.Type)
    {
    case SHADER_TYPE_BOOL:
        shader.setBool(name, value.Bool);
        break;
    case SHADER_TYPE_INT:
        shader.setInt(name, value.Int);
        break;
    case SHADER_TYPE_FLOAT:
        shader.setFloat(name, value.Float);
        break;
    case SHADER_TYPE_VEC2:
        shader.setVec2(name, value.Vec2);
        break;
    case SHADER_TYPE_VEC3:
        shader.setVec3(name, value.Vec3);
        break;
    case SHADER_TYPE_VEC4:
        shader.setVec4(name, value.Vec4);
    case SHADER_TYPE_COLOR3:
        shader.setColor3(name, value.Color3);
    case SHADER_TYPE_COLOR4:
        shader.setColor4(name, value.Color4);
        break;
    case SHADER_TYPE_MAT2:
        shader.setMat2(name, value.Mat2);
        break;
    case SHADER_TYPE_MAT3:
        shader.setMat3(name, value.Mat3);
        break;
    case SHADER_TYPE_MAT4:
        shader.setMat4(name, value.Mat4);
        break;
    default:
        //Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
        break;
    }
}

void Fracture::RenderQueueNode::WriteUniformSampler(Shader shader, std::string name, std::shared_ptr<UniformValueSampler> value)
{
    switch (value->Type)
    {
    case SHADER_TYPE_SAMPLER2D:
        shader.setTexture(name, value->texture, value->Unit);
        break;
    case SHADER_TYPE_SAMPLERCUBE:
        shader.setCubeMap(name, value->texture->GetTextureID(), value->Unit);
        break;
        FRACTURE_ERROR("Unrecognized Uniform type set");
        break;
    }
}

#include "RenderQueueNode.h"
#include "Rendering/Shader.h"
#include "Rendering/Material.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderBucket.h"
#include "Rendering/RenderBatch.h"
#include "AssetManager/AssetManager.h"


Fracture::RenderQueueNode::RenderQueueNode(std::string name):FrameNode(name)
{
}

void Fracture::RenderQueueNode::execute(Renderer& renderer)
{
 

}

void Fracture::RenderQueueNode::AcceptBucket(RenderBucket* bucket)
{
	m_buckets.push_back(std::move(bucket));
}

void Fracture::RenderQueueNode::render(Renderer& renderer)
{
    //renderer.clear();
    renderer.setViewport(renderer.Width(), renderer.Height());
  
    for (auto& bucket : m_buckets)
    {
        if (bucket->mType == BucketType::Transparent)
        {
            glDisable(GL_CULL_FACE);
        }
        else
        {
            glEnable(GL_CULL_FACE);
        }

        for (const auto& batch : bucket->getRenderBatches())
        {
            NumberBatches += 1;
            std::shared_ptr<Material> material = AssetManager::getMaterial(batch.first);
            material->use();

            auto* uniforms = material->GetUniforms();
            for (auto it = uniforms->begin(); it != uniforms->end(); ++it)
            {
                WriteUniformData(*material->getShader(), it->first, it->second);
            }

            std::unordered_map<std::string, std::shared_ptr<UniformValueSampler>>* uniformsSamplers = material->GetSamplerUniforms();
            for (auto it = uniformsSamplers->begin(); it != uniformsSamplers->end(); ++it)
            {
                WriteUniformSampler(*material->getShader(), it->first, it->second);
            }

            renderer.SetupLighting(material.get());

            for (auto command : batch.second->m_commnads)
            {
                renderer.Submit(command);
            }

        }
        if (bucket->mType == BucketType::Transparent)
        {
            glEnable(GL_CULL_FACE);
        }
    }
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
        shader.setCubeMap(name, value->id, value->Unit);
        break;
        FRACTURE_ERROR("Unrecognized Uniform type set");
        break;
    }
}

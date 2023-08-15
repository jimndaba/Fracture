#include "FracturePCH.h"
#include "ShadowCasterPass.h"
#include "World/SceneManager.h"
#include "Assets/AssetManager.h"
#include "Rendering/Mesh.h"
#include "Rendering/GraphicsDevice.h"
#include "Rendering/Material.h"
#include "Animation/AnimationSystem.h"

Fracture::ShadowCasterPass::ShadowCasterPass(const std::string& name, RenderContext* context, const ShadowCasterPassDef& info):
	IPass(name,context),
	Properties(info)
{

}


void Fracture::ShadowCasterPass::Setup()
{
    auto farPlane = SceneManager::ActiveCamera()->Far;
	shadowCascadeLevels = { (farPlane / 85.0f), (farPlane / 50.0f), (farPlane / 25.0f), (farPlane / 15.0f) };
    GraphicsDevice::Instance()->RenderSettings.ShadowCascadeCont = shadowCascadeLevels.size();
    
    Fracture::RenderTargetCreationInfo info;
    {
        Fracture::TextureCreationInfo desc;
        desc.Width = GraphicsDevice::RenderSettings.Shadow_Resolution;
        desc.Height = GraphicsDevice::RenderSettings.Shadow_Resolution;
        desc.TextureTarget = TextureTarget::Texture2DArray;
        desc.AttachmentTrgt = Fracture::AttachmentTarget::Depth;
        desc.format = Fracture::TextureFormat::DepthComponent;
        desc.internalFormat = Fracture::InternalFormat::DepthComponent32f;
        desc.formatType = Fracture::TextureFormatType::Float;
        desc.minFilter = TextureMinFilter::Linear;
        desc.magFilter = TextureMagFilter::Linear;
        desc.TextureArrayLevels = shadowCascadeLevels.size();
        desc.Name = "Shadows";
        desc.Wrap = TextureWrap::CampToBorder;

        info.Width = GraphicsDevice::RenderSettings.Shadow_Resolution;
        info.Height = GraphicsDevice::RenderSettings.Shadow_Resolution;
        info.Target = AttachmentTarget::Depth;
        info.DepthStencilAttachments.push_back(desc);
    }    
    GraphicsDevice::Instance()->CreateGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalDirectShadows, info);

    

    {
        BufferDescription desc;
        desc.bufferType = BufferType::UniformBuffer;
        desc.data = NULL;
        desc.size = sizeof(glm::mat4x4) * 16;
        desc.usage = BufferUsage::Static;
        mMatrixBuffer = std::make_shared<Buffer>();
        GraphicsDevice::Instance()->CreateBuffer(mMatrixBuffer.get(), desc);
        GraphicsDevice::Instance()->SetBufferIndexRange(mMatrixBuffer.get(), (int)ShaderUniformIndex::ShadowMatrix,0);
    }
    {
        BufferDescription desc;
        desc.bufferType = BufferType::UniformBuffer;
        desc.data = NULL;
        desc.size = (sizeof(shadowCascadeLevels[0]) * shadowCascadeLevels.size()) * sizeof(shadowCascadeLevels[0]);
        desc.usage = BufferUsage::Static;
        mSplaneDistances = std::make_shared<Buffer>();
        GraphicsDevice::Instance()->CreateBuffer(mSplaneDistances.get(), desc);
        GraphicsDevice::Instance()->SetBufferIndexRange(mSplaneDistances.get(), (int)ShaderUniformIndex::ShadowPlanes, 0);
    }

    {
        mShader = AssetManager::Instance()->GetShader("CascadeShadows");
        mShaderSkinned = AssetManager::Instance()->GetShader("CascadeShadowsSkinned");
    }
}


void Fracture::ShadowCasterPass::Execute()
{
    {
        auto lightMatrices = getLightSpaceMatrices(SceneManager::ActiveCamera().get());
        GraphicsDevice::Instance()->UpdateBufferData(mMatrixBuffer.get(), 0, sizeof(glm::mat4x4) * 16, &lightMatrices[0]);
    }
    //Update Cascade Shadow Levels
    {
        auto farPlane = SceneManager::ActiveCamera()->Far;
        shadowCascadeLevels = { (farPlane / 50.0f), (farPlane / 25.0f), (farPlane / 10.0f), (farPlane / 2.0f) };
        for (int i = 0; i < shadowCascadeLevels.size(); i++)
        {
            GraphicsDevice::Instance()->UpdateBufferData(mSplaneDistances.get(), i * shadowCascadeLevels.size() * sizeof(shadowCascadeLevels[0]), sizeof(shadowCascadeLevels[0]), &shadowCascadeLevels[i]);
        }
    }
    {
        const auto& global_shadow = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalDirectShadows);

        if (!global_shadow)
            return;

        RenderCommands::SetRenderTarget(Context, global_shadow);
        
        RenderCommands::SetViewport(Context, GraphicsDevice::RenderSettings.Shadow_Resolution, GraphicsDevice::RenderSettings.Shadow_Resolution, 0, 0);
        RenderCommands::SetScissor(Context, GraphicsDevice::RenderSettings.Shadow_Resolution, GraphicsDevice::RenderSettings.Shadow_Resolution, 0, 0);
        RenderCommands::ClearTarget(Context, (uint32_t)ClearBufferBit::Depth);

        RenderCommands::SetCullMode(Context, CullMode::Back);
        RenderCommands::Enable(Context, Fracture::GLCapability::DepthTest);
        RenderCommands::SetColorMask(Context, 0, 0, 0, 1);
        if (Context->mBatches.empty() && Context->ShadowDrawCalls.empty())
        {
            RenderCommands::ReleaseRenderTarget(Context);
            RenderCommands::SetColorMask(Context, 1, 1, 1, 1);
            RenderCommands::Disable(Context, Fracture::GLCapability::DepthTest);
            return;
        }

        for (auto& batches : Context->mBatches)
        {
            if (batches.second.empty())
                continue;

           

            if (!AssetManager::Instance()->IsMaterialLoaded(batches.first))
            {
                AssetManager::Instance()->AsyncLoadMaterialByID(batches.first);
                continue;
            }

            const auto& material = AssetManager::Instance()->GetMaterialByID(batches.first);
           
            if(material->IsSkinned)
                Fracture::RenderCommands::UseProgram(Context, mShaderSkinned->Handle);
            else
                Fracture::RenderCommands::UseProgram(Context, mShader->Handle);
            
            Fracture::RenderCommands::BindMaterial(Context, mShader.get(), material.get());

            //Set Shader
            for (auto batch : batches.second)
            {
                const auto& mesh = AssetManager::GetStaticByIDMesh(batch.first);

                Fracture::RenderCommands::BindVertexArrayObject(Context, batch.second->VAO);

                if (batch.second->ShadowDrawCalls.size())
                {
                    DrawElementsInstancedBaseVertex cmd;
                    cmd.basevertex = batch.second->OpaqueDrawCalls[0]->basevertex;
                    cmd.instancecount = batch.second->Transforms.size();
                    cmd.indices = batch.second->OpaqueDrawCalls[0]->SizeOfindices;
                    cmd.count = batch.second->OpaqueDrawCalls[0]->IndexCount;
                    Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
                }
                Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
            }


        }

        for (auto& drawCall : Context->ShadowDrawCalls)
        {
            if (!AssetManager::Instance()->IsMaterialLoaded(drawCall->MaterialID))
            {
                AssetManager::Instance()->AsyncLoadMaterialByID(drawCall->MaterialID);
                continue;
            }

            const auto& material = AssetManager::Instance()->GetMaterialByID(drawCall->MaterialID);

            if (!material->DepthWrite)
                continue;

            if (material->IsSkinned)
            {
                Fracture::RenderCommands::UseProgram(Context, mShaderSkinned->Handle);
                Fracture::RenderCommands::BindMaterial(Context, mShaderSkinned.get(), material.get());
                Fracture::RenderCommands::SetUniform(Context, mShaderSkinned.get(), "Model_matrix", drawCall->model);

                if (AnimationSystem::Instance()->HasGlobalPose(drawCall->EntityID))
                {
                    const auto& poses = AnimationSystem::Instance()->mGlobalPoses[drawCall->EntityID];
                    for (int i = 0; i < poses.GlobalPoses.size(); i++)
                    {
                        Fracture::RenderCommands::SetUniform(Context, mShaderSkinned.get(), "GlobalPose[" + std::to_string(i) + "]", poses.GlobalPoses[i]);
                    }
                }
            }
            else
            {
                Fracture::RenderCommands::UseProgram(Context, mShader->Handle);
                Fracture::RenderCommands::BindMaterial(Context, mShader.get(), material.get());
                Fracture::RenderCommands::SetUniform(Context, mShader.get(), "Model_matrix", drawCall->model);
            }

            Fracture::RenderCommands::BindVertexArrayObject(Context, drawCall->MeshHandle);

            DrawElementsInstancedBaseVertex cmd;
            cmd.basevertex = drawCall->basevertex;
            cmd.instancecount = 1;
            cmd.indices = drawCall->SizeOfindices;
            cmd.count = drawCall->IndexCount;
            Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
            Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
        }

        Fracture::RenderCommands::UseProgram(Context, 0);
        RenderCommands::ReleaseRenderTarget(Context);
        RenderCommands::SetColorMask(Context, 1, 1, 1, 1);
        RenderCommands::Disable(Context, Fracture::GLCapability::DepthTest);

    }


}

std::vector<glm::vec4>  Fracture::ShadowCasterPass::getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view)
{
    const auto inv = glm::inverse(proj * view);

    std::vector<glm::vec4> frustumCorners;
    for (unsigned int x = 0; x < 2; ++x)
    {
        for (unsigned int y = 0; y < 2; ++y)
        {
            for (unsigned int z = 0; z < 2; ++z)
            {
                const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                frustumCorners.push_back(pt / pt.w);
            }
        }
    }

    return frustumCorners;
}

glm::mat4  Fracture::ShadowCasterPass::getLightSpaceMatrix(const float fov, const float Cnear, const float Cfar, const glm::mat4 view,glm::vec3 cam_pos, glm::vec3 cam_forward)
{
    const auto& Lights = SceneManager::GetAllComponents<SunlightComponent>();

    if (Lights.empty())
        return glm::mat4(1.0f);

    const auto& transform = SceneManager::GetComponent<TransformComponent>(Lights[0]->entity);

    if (!transform)
        return glm::mat4(1.0f);

    const auto proj = glm::perspective(
        glm::radians(fov), (float)1920 / (float)1080, Cnear,
        Cfar);
    const auto corners = getFrustumCornersWorldSpace(proj, view);
    
    glm::vec3 center = glm::vec3(0, 0, 0);

    for (const auto& v : corners)
    {
        center += glm::vec3(v);
      
    }
    center /= corners.size();

   // glm::vec3 lightdir = glm::normalize(glm::degrees(glm::eulerAngles(transform->Rotation)));

    glm::vec3 lightdir = glm::normalize(-glm::eulerAngles(transform->Rotation));
    
    glm::vec3 eye = glm::vec3(lightdir.x, lightdir.y, lightdir.z);
    const auto lightView = glm::lookAt( center + eye,  center, glm::vec3(0.0f, 1.0f, 0.0f));

    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();
    float minZ = std::numeric_limits<float>::max();
    float maxZ = std::numeric_limits<float>::min();
    for (const auto& v : corners)
    {
        const auto trf = lightView * v;
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);
    }

    // Tune this parameter according to the scene
    constexpr float zMult = 10.0f;
    if (minZ < 0)
    {
        minZ *= zMult;
    }
    else
    {
        minZ /= zMult;
    }
    if (maxZ < 0)
    {
        maxZ /= zMult;
    }
    else
    {
        maxZ *= zMult;
    }

    const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

    return lightProjection * lightView;
}


std::vector<glm::mat4>  Fracture::ShadowCasterPass::getLightSpaceMatrices(CameraComponent* camera)
{
    std::vector<glm::mat4> ret;
    for (size_t i = 0; i < shadowCascadeLevels.size() + 1; ++i)
    {
        if (i == 0)
        {
            ret.push_back(getLightSpaceMatrix(camera->FoV, camera->Near, shadowCascadeLevels[i], camera->ViewMatrix,camera->Position,camera->Front));
        }
        else if (i < shadowCascadeLevels.size())
        {
            ret.push_back(getLightSpaceMatrix(camera->FoV, shadowCascadeLevels[i - 1], shadowCascadeLevels[i], camera->ViewMatrix,camera->Position, camera->Front));
        }
        else
        {
            ret.push_back(getLightSpaceMatrix(camera->FoV, shadowCascadeLevels[i - 1], camera->Far, camera->ViewMatrix, camera->Position, camera->Front));
        }
    }
    return ret;
}

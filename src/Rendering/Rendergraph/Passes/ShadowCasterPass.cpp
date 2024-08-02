#include "FracturePCH.h"
#include "ShadowCasterPass.h"


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
    
    //Cascade Shadow Maps
    {
        Fracture::RenderTargetCreationInfo info;
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
        info.HasDebugViews = true;
        info.NoDebugViews = shadowCascadeLevels.size();
        info.DepthStencilAttachments.push_back(desc);
        GraphicsDevice::Instance()->CreateGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalDirectShadows, info);
    }    
    //Spot Shadow Maps
    {
        Fracture::RenderTargetCreationInfo sinfo;       
        Fracture::TextureCreationInfo sdesc;
        sdesc.Width = 512;
        sdesc.Height = 512;
        sdesc.TextureTarget = TextureTarget::Texture2DArray;
        sdesc.AttachmentTrgt = Fracture::AttachmentTarget::Depth;
        sdesc.format = Fracture::TextureFormat::DepthComponent;
        sdesc.internalFormat = Fracture::InternalFormat::DepthComponent32f;
        sdesc.formatType = Fracture::TextureFormatType::Float;
        sdesc.minFilter = TextureMinFilter::Linear;
        sdesc.magFilter = TextureMagFilter::Linear;
        sdesc.TextureArrayLevels = 10;
        sdesc.Name = "SpotShadows";
        sdesc.Wrap = TextureWrap::CampToBorder;

        sinfo.Width = sdesc.Width;
        sinfo.Height = sdesc.Height;
        sinfo.Target = AttachmentTarget::Depth;
        sinfo.DepthStencilAttachments.push_back(sdesc);
        GraphicsDevice::Instance()->CreateGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalSpotShadows, sinfo);
    }
    //Point Shadow Maps
    {
       //Fracture::RenderTargetCreationInfo info;
       //Fracture::TextureCreationInfo desc;
       //desc.Width = 256;
       //desc.Height = 256;
       //desc.TextureTarget = TextureTarget::TextureCubeMapArray;
       //desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
       //desc.format = Fracture::TextureFormat::RGB;
       //desc.internalFormat = Fracture::InternalFormat::RGB16F;
       //desc.formatType = Fracture::TextureFormatType::Float;
       //desc.minFilter = TextureMinFilter::Linear;
       //desc.magFilter = TextureMagFilter::Linear;
       //desc.Wrap = TextureWrap::ClampToEdge;
       //desc.Name = "LightProbeTexture";
       //desc.TextureArrayLevels = 20 * 6;      
       //desc.Name = "Shadows";
       //desc.Wrap = TextureWrap::CampToBorder;   
       //info.Width = 256;
       //info.Height = 256;
       //info.Target = AttachmentTarget::Depth;
       //info.DepthStencilAttachments.push_back(desc);
       //GraphicsDevice::Instance()->CreateGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalPointShadows, info);
    }
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
        mShaderInstanced = AssetManager::Instance()->GetShader("CascadeShadowsInstanced");
        mSpotShader = AssetManager::Instance()->GetShader("SpotShadows");
        //mPointShader = AssetManager::Instance()->GetShader("PointShadows");
        mShaderSkinned = AssetManager::Instance()->GetShader("CascadeShadowsSkinned");
    }
}

void Fracture::ShadowCasterPass::Execute()
{
    OPTICK_EVENT();
    {
        if (true)
        {
            auto lightMatrices = getLightSpaceMatrices(SceneManager::ActiveCamera().get());
            GraphicsDevice::Instance()->UpdateBufferData(mMatrixBuffer.get(), 0, sizeof(glm::mat4x4) * 16, &lightMatrices[0]);

            //Update Cascade Shadow Levels
            {
                auto farPlane = SceneManager::ActiveCamera()->Far;
                shadowCascadeLevels = { (farPlane / 50.0f), (farPlane / 25.0f), (farPlane / 10.0f), (farPlane / 2.0f) };
                for (int i = 0; i < shadowCascadeLevels.size(); i++)
                {
                    GraphicsDevice::Instance()->UpdateBufferData(mSplaneDistances.get(), i * shadowCascadeLevels.size() * sizeof(shadowCascadeLevels[0]), sizeof(shadowCascadeLevels[0]), &shadowCascadeLevels[i]);
                }
            }
            IsDirty = false;
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

        RenderCommands::SetCullMode(Context, CullMode::Front);
        RenderCommands::Enable(Context, Fracture::GLCapability::DepthTest);
        RenderCommands::SetColorMask(Context, 0, 0, 0, 1);
        if (Context->mBatches.empty() && Context->ShadowDrawCalls.empty())
        {
            RenderCommands::ReleaseRenderTarget(Context);
            RenderCommands::SetColorMask(Context, 1, 1, 1, 1);
            RenderCommands::Disable(Context, Fracture::GLCapability::DepthTest);
            return;
        }

        for (auto& drawCall : Context->ShadowDrawCalls)
        {
            if (!AssetManager::Instance()->IsMaterialLoaded(drawCall->MaterialID))
            {
                AssetManager::Instance()->AsyncLoadMaterialByID(drawCall->MaterialID);
                continue;
            }

            const auto& material = AssetManager::Instance()->GetMaterialByID(drawCall->MaterialID);
            if (material->IsTranslucent)
            {
                continue;
            }


            if (!material->CastsShadows)
            {
                continue;
            }

            Shader* current_shader;
            if (material->IsSkinned)
                current_shader = mShaderSkinned.get();
            if (material->IsInstanced)
                current_shader = mShaderInstanced.get();
            else
                current_shader = mShader.get();

            
            Fracture::RenderCommands::UseProgram(Context, current_shader->Handle);
            Fracture::RenderCommands::BindMaterial(Context, current_shader, material.get());
            Fracture::RenderCommands::SetUniform(Context, current_shader, "Model_matrix", drawCall->model);
            Fracture::RenderCommands::SetUniform(Context, current_shader, "MaterialIndex", (int)drawCall->GPUMaterialIndex);

            if (material->IsSkinned)
            {                
                if (AnimationSystem::Instance()->mGlobalPoseMatrices.find(drawCall->EntityID) != AnimationSystem::Instance()->mGlobalPoseMatrices.end())
                {
                    Fracture::RenderCommands::SetUniform(Context, current_shader, "IsAnimated", AnimationSystem::Instance()->IsPlaying);
                    const auto& poses = AnimationSystem::Instance()->mGlobalPoseMatrices[drawCall->EntityID];
                    if (!poses.empty() && AnimationSystem::Instance()->IsPlaying)
                    {                        
                        GraphicsDevice::Instance()->UpdateAnimationData(poses);
                    }                    
                }
            }
            else
            { 
                Fracture::RenderCommands::SetUniform(Context, current_shader, "IsAnimated", false);
            }

            Fracture::RenderCommands::BindVertexArrayObject(Context, drawCall->MeshHandle);
            switch (drawCall->CallType)
            {
            case DrawCommandType::DrawElementsInstancedBaseVertex:
            {
                DrawElementsInstancedBaseVertex cmd;
                cmd.mode = drawCall->DrawCallPrimitive;
                cmd.basevertex = drawCall->basevertex;
                cmd.instancecount = 1;
                cmd.indices = drawCall->SizeOfindices;
                cmd.count = drawCall->IndexCount;
                Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
                break;
            }
            case DrawCommandType::MultiDrawElementsIndirect:
            {
                DrawElementsInstancedBaseVertex cmd;
                Fracture::RenderCommands::BindBuffer(Context, BufferType::DrawIndirectBuffer, GraphicsDevice::Instance()->GetIndirectBuffer());
                Fracture::RenderCommands::DrawElementsIndirect(Context, nullptr, Context->IndirectTerrains.size(), 0);
                Fracture::RenderCommands::BindBuffer(Context, BufferType::DrawIndirectBuffer, 0);
                break;
            }

            case DrawCommandType::DrawArrys:
            {
                DrawArray cmd;
                cmd.count = drawCall->IndexCount;
                cmd.first = drawCall->baseIndex;
                cmd.mode = drawCall->DrawCallPrimitive;
                //Fracture::RenderCommands::DrawArray(Context, cmd);
                break;
            }

            }
            Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
            Fracture::RenderCommands::ResetTextureUnits(Context, current_shader);
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
            if (material->IsTranslucent)
            {
                continue;
            }

            if (!material->CastsShadows)
            {
                continue;
            }

            Shader* current_shader = nullptr;
            if (material->IsSkinned)
            {
                current_shader = mShaderSkinned.get();
            }
            if (material->IsInstanced)
            {
                current_shader = mShaderInstanced.get();
            }
            if (!material->IsInstanced)
            {
                current_shader = mShader.get();
            }

            if (!current_shader)
                continue;
            //Set Shader
            for (auto batch : batches.second)
            {
                if (batch.second->Transforms.empty())
                {
                    continue;
                }
                   
                Fracture::RenderCommands::UseProgram(Context, current_shader->Handle);
                Fracture::RenderCommands::BindMaterial(Context, current_shader, material.get());
                Fracture::RenderCommands::SetUniform(Context, current_shader, "IsAnimated", false);
                Fracture::RenderCommands::SetUniform(Context, current_shader, "MaterialIndex", (int)batch.second->GPUMaterialIndex);  
                Fracture::RenderCommands::BindVertexArrayObject(Context, batch.second->VAO);

                DrawElementsInstancedBaseVertex cmd;
                cmd.mode = batch.second->DrawCallPrimitive;
                cmd.basevertex = batch.second->basevertex;
                cmd.instancecount = batch.second->Transforms.size();
                cmd.indices = batch.second->SizeOfindices;
                cmd.count = batch.second->IndexCount;
                Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);

                Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
            }
            Fracture::RenderCommands::ResetTextureUnits(Context, current_shader);
        }


    }

    //SpotShadows
    {
        const auto& tshadow = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalSpotShadows);
        const auto& lightlist = SceneManager::GetAllComponents<SpotlightComponent>();
        
        if (!tshadow)
            return;

        RenderCommands::SetRenderTarget(Context, tshadow);
        RenderCommands::SetViewport(Context, 512, 512, 0, 0);
        RenderCommands::SetScissor(Context, 512, 512, 0, 0);
     
        if (Context->mBatches.empty() && Context->ShadowDrawCalls.empty())
        {
            RenderCommands::ReleaseRenderTarget(Context);
            RenderCommands::SetColorMask(Context, 1, 1, 1, 1);
            RenderCommands::Disable(Context, Fracture::GLCapability::DepthTest);
            return;
        }

        Fracture::RenderCommands::UseProgram(Context, mSpotShader->Handle);

        

        std::vector<glm::mat4> mSpotMatricies;
        mSpotMatricies.resize(10);      
        for (int i = 0; i < lightlist.size(); i++)
        {
            if (i > mSpotMatricies.size())
                continue;

            const auto& transform = SceneManager::GetComponent<TransformComponent>(lightlist[i]->GetID());
            float outerConeAngleRadians = glm::radians(lightlist[i]->OutCutoff);
            // Calculate the full field of view
            float fov = 2.0f * outerConeAngleRadians;

            glm::mat4 projection = glm::perspective(fov, 1.0f, 0.1f, lightlist[i]->Strength);
            
            glm::vec3 defaultDirection = glm::vec3(0.0f, 0.0f, -1.0f);
            glm::vec3 spotlightDirection = glm::normalize(glm::rotate(transform->Rotation, defaultDirection));

            glm::mat4 view = glm::lookAt(transform->Position, transform->Position + spotlightDirection, glm::vec3(0, 1, 0));
            mSpotMatricies[i] = projection * view;
        }

        RenderCommands::SetCullMode(Context, CullMode::None);
       
        for (int i = 0; i < lightlist.size(); i++)
        {
            if (i > mSpotMatricies.size())
                continue;
           
            Fracture::RenderCommands::FrameBufferDepthTextureTarget(Context, tshadow->Handle, 0, i, tshadow->DepthStencilAttachment->Handle, 0);
            RenderCommands::ClearTarget(Context, (uint32_t)ClearBufferBit::Depth);

    
            Fracture::RenderCommands::SetUniform(Context, mSpotShader.get(), "LightMatrix", mSpotMatricies[i]);

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
                if (material->IsTranslucent)
                {
                    continue;
                }

                if (!material->CastsShadows)
                {
                    continue;
                }

                uint32_t handle;
                if (material->IsSkinned)
                   handle = mShaderSkinned->Handle;
                else
                   handle = mSpotShader->Handle;

                Fracture::RenderCommands::UseProgram(Context, handle);
                Fracture::RenderCommands::BindMaterial(Context, mSpotShader.get(), material.get());

                //Set Shader
                for (auto batch : batches.second)
                {

                    Fracture::RenderCommands::SetUniform(Context, mShader.get(), "MaterialIndex", (int)batch.second->GPUMaterialIndex);
                    Fracture::RenderCommands::BindVertexArrayObject(Context, batch.second->VAO);             
                    if (batch.second->Transforms.size())
                    {                      
                        DrawElementsInstancedBaseVertex cmd;
                        cmd.mode = batch.second->DrawCallPrimitive;
                        cmd.basevertex = batch.second->basevertex;
                        cmd.instancecount = batch.second->Transforms.size();
                        cmd.indices = batch.second->SizeOfindices;
                        cmd.count = batch.second->IndexCount;
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

                if (material->IsTranslucent)
                {
                    continue;
                }


                if (!material->DepthWrite)
                    continue;

                if (material->IsSkinned)
                {
                    Fracture::RenderCommands::UseProgram(Context, mShaderSkinned->Handle);
                    Fracture::RenderCommands::BindMaterial(Context, mShaderSkinned.get(), material.get());
                    Fracture::RenderCommands::SetUniform(Context, mShaderSkinned.get(), "Model_matrix", drawCall->model);

                    Fracture::RenderCommands::SetUniform(Context, mShader.get(), "MaterialIndex", (int)drawCall->GPUMaterialIndex);

                    if (AnimationSystem::Instance()->mGlobalPoseMatrices.find(drawCall->EntityID) != AnimationSystem::Instance()->mGlobalPoseMatrices.end())
                    {
                        const auto& poses = AnimationSystem::Instance()->mGlobalPoseMatrices[drawCall->EntityID];
                        if (!poses.empty() && AnimationSystem::Instance()->IsPlaying)
                        {
                            Fracture::RenderCommands::SetUniform(Context, mShaderSkinned.get(), "IsAnimated", true);
                            GraphicsDevice::Instance()->UpdateAnimationData(poses);
                        }
                        else
                        {
                            Fracture::RenderCommands::SetUniform(Context, mShaderSkinned.get(), "IsAnimated", false);
                        }
                    }
                }
                else
                {
                    Fracture::RenderCommands::UseProgram(Context, mShader->Handle);
                    Fracture::RenderCommands::BindMaterial(Context, mShader.get(), material.get());
                    Fracture::RenderCommands::SetUniform(Context, mShader.get(), "Model_matrix", drawCall->model);
                    Fracture::RenderCommands::SetUniform(Context, mShader.get(), "IsAnimated", false);

                    Fracture::RenderCommands::SetUniform(Context, mShader.get(), "MaterialIndex", (int)drawCall->GPUMaterialIndex);

                }

                Fracture::RenderCommands::BindVertexArrayObject(Context, drawCall->MeshHandle);

                switch (drawCall->CallType)
                {
                case DrawCommandType::DrawElementsInstancedBaseVertex:
                {
                    DrawElementsInstancedBaseVertex cmd;
                    cmd.mode = drawCall->DrawCallPrimitive;
                    cmd.basevertex = drawCall->basevertex;
                    cmd.instancecount = 1;
                    cmd.indices = drawCall->SizeOfindices;
                    cmd.count = drawCall->IndexCount;
                    Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
                    break;
                }               

                case DrawCommandType::MultiDrawElementsIndirect:
                {
                    DrawElementsInstancedBaseVertex cmd;
                    Fracture::RenderCommands::BindBuffer(Context, BufferType::DrawIndirectBuffer, GraphicsDevice::Instance()->GetIndirectBuffer());
                    Fracture::RenderCommands::DrawElementsIndirect(Context, nullptr, Context->IndirectTerrains.size(), 0);
                    Fracture::RenderCommands::BindBuffer(Context, BufferType::DrawIndirectBuffer, 0);
                    break;
                }

                case DrawCommandType::DrawArrys:
                {
                    DrawArray cmd;
                    cmd.count = drawCall->IndexCount;
                    cmd.first = drawCall->baseIndex;
                    cmd.mode = drawCall->DrawCallPrimitive;
                    Fracture::RenderCommands::DrawArray(Context, cmd);
                    break;
                }
          
                Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
                }
            }
        }
        Fracture::RenderCommands::ResetTextureUnits(Context, mSpotShader.get());
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

    glm::vec3 defaultDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 direction = -glm::normalize(glm::rotate(transform->Rotation, defaultDirection));
    const auto lightView = glm::lookAt( center + direction,  center, glm::vec3(0.0f, 1.0f, 0.0f));

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

#include "FracturePCH.h"
#include "LightProbeSystem.h"
#include "Assets/AssetManager.h"
#include "SceneManager.h"
#include "Rendering/GraphicsDevice.h"
#include "Rendering/Material.h"

Fracture::LightProbeSystem::LightProbeSystem()
{   

}


void Fracture::LightProbeSystem::Bake(RenderContext* Context, UUID id)
{
    const auto& lightProbe = SceneManager::GetComponent<LightProbeComponent>(id);
    if (!lightProbe)
        return;

    DoIrradiance(Context, id);

    if (lightProbe->ProbeType == LightProbeComponent::LightProbeType::Global)
    {
        DoPrefitler(Context, id);

        DoBRDF(Context, id);
    }
   
}

void Fracture::LightProbeSystem::DoIrradiance(RenderContext* Context, UUID component)
{
    const auto& lightProbe = SceneManager::GetComponent<LightProbeComponent>(component);
    if (!lightProbe)
        return;

 
    if (!lightProbe->IsInterior && lightProbe->ProbeType == LightProbeComponent::LightProbeType::Global)
    {
        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };


        const auto& target = GraphicsDevice::Instance()->GetGlobalRenderTarget("Global_IrradianceBuffer");
        const auto& shader = AssetManager::Instance()->GetShader("irradianceShader");

        //* DRAW IRADDIANCE MAP
        Fracture::RenderCommands::UseProgram(Context, shader->Handle);
        Fracture::RenderCommands::Disable(Context, Fracture::GLCapability::FaceCulling);
        Fracture::RenderCommands::SetRenderTarget(Context, target->Handle);
        Fracture::RenderCommands::SetRenderBuffer(Context, target->RenderBufferHandle);
        Fracture::RenderCommands::RenderBufferTextureStorage(Context, target->RenderBufferHandle, InternalFormat::DepthComponent24, lightProbe->LightProbeResolution, lightProbe->LightProbeResolution);


        Fracture::RenderCommands::SetViewport(Context, lightProbe->LightProbeResolution, lightProbe->LightProbeResolution, 0, 0);
        Fracture::RenderCommands::SetScissor(Context, lightProbe->LightProbeResolution, lightProbe->LightProbeResolution, 0, 0);

        Fracture::RenderCommands::SetTexture(Context, shader.get(), "aSkyMap", GraphicsDevice::Instance()->GetGlobalTexture("Global_SkyMap")->Handle, 0);
        Fracture::RenderCommands::SetUniform(Context, shader.get(), "projection_matrix", captureProjection);
        if (GraphicsDevice::Instance()->GetGlobalTexture("Global_SkyMap") && lightProbe->ProbeType == LightProbeComponent::LightProbeType::Global)
        {
            for (unsigned int i = 0; i < 6; ++i)
            {
                Fracture::RenderCommands::SetUniform(Context, shader.get(), "view_matrix", captureViews[i]);
                Fracture::RenderCommands::FrameBufferTextureTarget(Context, target->Handle, 0, i, GraphicsDevice::Instance()->GetIrradianceMap(lightProbe->GetID()), 0);
                Fracture::RenderCommands::ClearTarget(Context, (uint32_t)Fracture::ClearFlags::Color | (uint32_t)Fracture::ClearFlags::Depth);
                GraphicsDevice::Instance()->RenderCaptureCube(Context); // renders a 1x1 cube          
            }
            lightProbe->IsBaked = true;
            Fracture::RenderCommands::SetRenderTarget(Context, (uint32_t)0);
        }
        else
        {
            FRACTURE_ERROR("No Skybox Texture Found");
        }

        Fracture::RenderCommands::UseProgram(Context, 0);
    }

   
    if (lightProbe->ProbeType == LightProbeComponent::LightProbeType::Local)
    {
        //Capture World Diffuse
        {
            const auto& shader = AssetManager::Instance()->GetShader("ProbeDiffuse");

            if (!shader)
                FRACTURE_ERROR("Missing Probe Shader");

           
            const auto& target = GraphicsDevice::Instance()->GetGlobalRenderTarget(GlobalRenderTargets::GlobalIrradiance);
          
            Fracture::RenderCommands::SetRenderTarget(Context, target->Handle);              
            Fracture::RenderCommands::SetViewport(Context, 32, 32, 0, 0);
            Fracture::RenderCommands::SetScissor(Context, 32, 32, 0, 0);


            Fracture::RenderCommands::ClearColor(Context, Fracture::Colour::Black);
            Fracture::RenderCommands::SetRenderTarget(Context, target->Handle);
            Fracture::RenderCommands::ClearTarget(Context, (uint32_t)Fracture::ClearFlags::Color | (uint32_t)Fracture::ClearFlags::Depth);
         

            
            Fracture::RenderCommands::UseProgram(Context, shader->Handle);
            Fracture::RenderCommands::SetUniform(Context, shader.get(), "MaxProbes", (int)lightProbe->ProbePositions.size());

            for (int probe = 0; probe < lightProbe->ProbePositions.size(); probe++)
            {
                
                Fracture::RenderCommands::SetUniform(Context, shader.get(), "iLayer", probe);

                std::vector<glm::mat4> captureViews;
                glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 50.0f);

                auto position = glm::vec3(lightProbe->ProbePositions[probe].x, lightProbe->ProbePositions[probe].y, lightProbe->ProbePositions[probe].z);
                captureViews.push_back(captureProjection * glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
                captureViews.push_back(captureProjection * glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
                captureViews.push_back(captureProjection * glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
                captureViews.push_back(captureProjection * glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
                captureViews.push_back(captureProjection * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
                captureViews.push_back(captureProjection * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

                for (unsigned int face = 0; face < 6; ++face)
                {
                    Fracture::RenderCommands::SetUniform(Context, shader.get(), "LightProbeMatricies[" + std::to_string(face) + "]", captureViews[face]);                                   
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
                    RenderCommands::SetCullMode(Context, material->cullmode);

                    Fracture::RenderCommands::BindMaterial(Context, shader.get(), material.get());

                    if (material->IsTranslucent)
                    {
                        RenderCommands::Enable(Context, Fracture::GLCapability::Blending);
                        RenderCommands::BlendFunction(Context, Fracture::BlendFunc::SrcAlpha, Fracture::BlendFunc::OneMinusSrcAlpha);
                    }

                    if (GraphicsDevice::Instance()->RenderSettings.SSAO_Enabled)
                    {
                        const auto& global_SSAO = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalSSAO);
                        Fracture::RenderCommands::SetTexture(Context, shader.get(), "aGlobalAO", global_SSAO->ColorAttachments[0]->Handle, (int)TEXTURESLOT::GlobalAO);
                    }
                    {
                        const auto& global_Shadows = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalDirectShadows);
                        Fracture::RenderCommands::SetTexture(Context, shader.get(), "aShadowMap", global_Shadows->DepthStencilAttachment->Handle, (int)TEXTURESLOT::DirectShadows);
                    }




                    if (batches.second.size())
                    {
                        //Set Shader
                        for (auto batch : batches.second)
                        {
                            const auto& mesh = AssetManager::GetStaticByIDMesh(batch.first);

                            Fracture::RenderCommands::BindVertexArrayObject(Context, batch.second->VAO);

                            std::vector<std::shared_ptr<MeshDrawCall>> drawcalls;
                            if (material->IsTranslucent)
                                drawcalls = batch.second->TransparentDrawCalls;
                            else
                                drawcalls = batch.second->OpaqueDrawCalls;

                            if (drawcalls.size())
                            {
                                DrawElementsInstancedBaseVertex cmd;
                                cmd.basevertex = drawcalls[0]->basevertex;
                                cmd.instancecount = batch.second->Transforms.size();
                                cmd.indices = drawcalls[0]->SizeOfindices;
                                cmd.count = drawcalls[0]->IndexCount;
                                Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
                            }
                            Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
                        }
                    }


                    if (material->IsTranslucent)
                        RenderCommands::Disable(Context, Fracture::GLCapability::Blending);
                }


                /*

                for (int face = 0; face < 6; ++face)
                {   
                    Fracture::RenderCommands::SetUniform(Context, shader.get(), "iLayer", face);
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
                        RenderCommands::SetCullMode(Context, material->cullmode);

                        Fracture::RenderCommands::BindMaterial(Context, shader.get(), material.get());

                        if (material->IsTranslucent)
                        {
                            RenderCommands::Enable(Context, Fracture::GLCapability::Blending);
                            RenderCommands::BlendFunction(Context, Fracture::BlendFunc::SrcAlpha, Fracture::BlendFunc::OneMinusSrcAlpha);
                        }

                        if (GraphicsDevice::Instance()->RenderSettings.SSAO_Enabled)
                        {
                            const auto& global_SSAO = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalSSAO);
                            Fracture::RenderCommands::SetTexture(Context, shader.get(), "aGlobalAO", global_SSAO->ColorAttachments[0]->Handle, (int)TEXTURESLOT::GlobalAO);
                        }
                        {
                            const auto& global_Shadows = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalDirectShadows);
                            Fracture::RenderCommands::SetTexture(Context, shader.get(), "aShadowMap", global_Shadows->DepthStencilAttachment->Handle, (int)TEXTURESLOT::DirectShadows);
                        }




                        if (batches.second.size())
                        {
                            //Set Shader


                            for (auto batch : batches.second)
                            {
                                Fracture::RenderCommands::FrameBufferTextureTarget(Context, target->Handle, 0, (int)(probe * 6 + face), GraphicsDevice::Instance()->GetLightProbeMap(), 0);

                                const auto& mesh = AssetManager::GetStaticByIDMesh(batch.first);

                                Fracture::RenderCommands::BindVertexArrayObject(Context, batch.second->VAO);

                                std::vector<std::shared_ptr<MeshDrawCall>> drawcalls;
                                if (material->IsTranslucent)
                                    drawcalls = batch.second->TransparentDrawCalls;
                                else
                                    drawcalls = batch.second->OpaqueDrawCalls;

                                if (drawcalls.size())
                                {
                                    DrawElementsInstancedBaseVertex cmd;
                                    cmd.basevertex = drawcalls[0]->basevertex;
                                    cmd.instancecount = batch.second->Transforms.size();
                                    cmd.indices = drawcalls[0]->SizeOfindices;
                                    cmd.count = drawcalls[0]->IndexCount;
                                    Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
                                }
                                Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
                            }
                        }


                        if (material->IsTranslucent)
                            RenderCommands::Disable(Context, Fracture::GLCapability::Blending);
                    }

                }       
                */
                Fracture::RenderCommands::ResetTextureUnits(Context, shader.get());
            }

            Fracture::RenderCommands::UseProgram(Context, 0);
        }
    
        {
            glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
            glm::mat4 captureViews[] =
            {
               glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
               glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
               glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
               glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
               glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
               glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
            };

           
           
            const auto& target = GraphicsDevice::Instance()->GetGlobalRenderTarget("Global_IrradianceBuffer");
            const auto& shader = AssetManager::Instance()->GetShader("irradianceShader");

            //* DRAW IRADDIANCE MAP
            Fracture::RenderCommands::UseProgram(Context, shader->Handle);
            Fracture::RenderCommands::Disable(Context, Fracture::GLCapability::FaceCulling);
            Fracture::RenderCommands::SetRenderTarget(Context, target->Handle);
            Fracture::RenderCommands::SetRenderBuffer(Context, target->RenderBufferHandle);
            Fracture::RenderCommands::RenderBufferTextureStorage(Context, target->RenderBufferHandle, InternalFormat::DepthComponent24, 32, 32);
            Fracture::RenderCommands::SetViewport(Context, 32, 32, 0, 0);
            Fracture::RenderCommands::SetScissor(Context, 32, 32, 0, 0);

            for (int probe = 0; probe < lightProbe->ProbePositions.size(); probe++)
            {
                Fracture::RenderCommands::SetTexture(Context, shader.get(), "aProbeMap", GraphicsDevice::Instance()->GetGlobalRenderTarget(GlobalRenderTargets::GlobalIrradiance)->ColorAttachments[0]->Handle, 0);
                Fracture::RenderCommands::SetUniform(Context, shader.get(), "_ProbeFlag", true);
                Fracture::RenderCommands::SetUniform(Context, shader.get(), "projection_matrix", captureProjection);
                Fracture::RenderCommands::SetUniform(Context, shader.get(), "ProbeID", probe);
                for (unsigned int i = 0; i < 6; ++i)
                {
                    int current_face = probe * 6 + i;
                    Fracture::RenderCommands::SetUniform(Context, shader.get(), "view_matrix", captureViews[i]);
                    Fracture::RenderCommands::FrameBufferTextureTarget(Context, target->Handle, 0, current_face, GraphicsDevice::Instance()->GetLightProbeIrradiance(), 0);
                    Fracture::RenderCommands::ClearTarget(Context, (uint32_t)Fracture::ClearFlags::Color | (uint32_t)Fracture::ClearFlags::Depth);
                    GraphicsDevice::Instance()->RenderCaptureCube(Context); // renders a 1x1 cube          
                }
                lightProbe->IsBaked = true;
            }

            Fracture::RenderCommands::SetRenderTarget(Context, (uint32_t)0);
            Fracture::RenderCommands::UseProgram(Context, 0);
        }
    }
    
}

void Fracture::LightProbeSystem::DoPrefitler(RenderContext* Context, UUID component)
{
    const auto& lightProbe = SceneManager::GetComponent<LightProbeComponent>(component);
    if (!lightProbe)
        return;

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };


    const auto& target = GraphicsDevice::Instance()->GetGlobalRenderTarget("Global_IrradianceBuffer");

    Fracture::RenderCommands::Disable(Context, Fracture::GLCapability::FaceCulling);
    Fracture::RenderCommands::SetRenderTarget(Context, target->Handle);
    Fracture::RenderCommands::SetRenderBuffer(Context, target->RenderBufferHandle);
    Fracture::RenderCommands::FrameBufferSetDrawBuffers(Context, target->Handle, 1);
    Fracture::RenderCommands::RenderBufferTextureStorage(Context, target->RenderBufferHandle, InternalFormat::DepthComponent24, lightProbe->BRDFResolution, lightProbe->BRDFResolution);
    
    Fracture::RenderCommands::SetViewport(Context, lightProbe->BRDFResolution, lightProbe->BRDFResolution, 0, 0);
    Fracture::RenderCommands::SetScissor(Context, lightProbe->BRDFResolution, lightProbe->BRDFResolution, 0, 0);
         
    if (GraphicsDevice::Instance()->GetGlobalTexture("Global_SkyMap") && lightProbe->ProbeType == LightProbeComponent::LightProbeType::Global)
    {
        const auto pre_shader = AssetManager::Instance()->GetShader("PrefilterBRDF");
        Fracture::RenderCommands::UseProgram(Context, pre_shader->Handle);

       
        Fracture::RenderCommands::SetUniform(Context, pre_shader.get(), "projection_matrix", captureProjection);
        Fracture::RenderCommands::SetTexture(Context, pre_shader.get(), "aSkyMap", GraphicsDevice::Instance()->GetGlobalTexture("Global_SkyMap")->Handle, 0);

        unsigned int maxMipLevels = 5;
        for (int mip = 0; mip < maxMipLevels; mip++)
        {
            unsigned int mipWidth = lightProbe->BRDFResolution * std::pow(0.5, mip);
            unsigned int mipHeight = lightProbe->BRDFResolution * std::pow(0.5, mip);

            float roughness = (float)mip / (float)(maxMipLevels - 1);
            Fracture::RenderCommands::SetUniform(Context, pre_shader.get(), "roughness", roughness);
            for (unsigned int i = 0; i < 6; ++i)
            {
                Fracture::RenderCommands::SetUniform(Context, pre_shader.get(), "view_matrix", captureViews[i]);
                Fracture::RenderCommands::FrameBufferTextureTarget(Context, target->Handle, 0, i, GraphicsDevice::Instance()->GetSpecularBRDFMap(lightProbe->GetID()), mip);
                Fracture::RenderCommands::ClearTarget(Context, (uint32_t)Fracture::ClearFlags::Color | (uint32_t)Fracture::ClearFlags::Depth);
                GraphicsDevice::Instance()->RenderCaptureCube(Context); // renders a 1x1 cube          
            }
        }
        lightProbe->IsBaked = true;
        Fracture::RenderCommands::SetRenderTarget(Context, (uint32_t)0);
    }
    else
    {
        FRACTURE_ERROR("No Skybox Texture Found");
    }
}

void Fracture::LightProbeSystem::DoBRDF(RenderContext* Context, UUID component)
{
    const auto& lightProbe = SceneManager::GetComponent<LightProbeComponent>(component);
    if (!lightProbe)
        return;

    const auto& target = GraphicsDevice::Instance()->GetGlobalRenderTarget("Global_IrradianceBuffer");
    Fracture::RenderCommands::Disable(Context, Fracture::GLCapability::FaceCulling); 
    Fracture::RenderCommands::SetRenderTarget(Context, target->Handle);
    Fracture::RenderCommands::SetRenderBuffer(Context, target->RenderBufferHandle);
    Fracture::RenderCommands::FrameBufferSetDrawBuffers(Context, target->Handle, 1);
    Fracture::RenderCommands::RenderBufferTextureStorage(Context, target->RenderBufferHandle, InternalFormat::DepthComponent24, lightProbe->BRDFLUTResolution, lightProbe->BRDFLUTResolution);
   
    Fracture::RenderCommands::SetViewport(Context, lightProbe->BRDFLUTResolution, lightProbe->BRDFLUTResolution, 0, 0);
    Fracture::RenderCommands::SetScissor(Context, lightProbe->BRDFLUTResolution, lightProbe->BRDFLUTResolution, 0, 0);


    Fracture::RenderCommands::FrameBufferAttachTexture(Context, target->Handle, 0, GraphicsDevice::Instance()->GetBRDFLUTMap(lightProbe->GetID()),0);

    const auto shader = AssetManager::Instance()->GetShader("BRDFLUT");
    Fracture::RenderCommands::UseProgram(Context,shader->Handle);
    Fracture::RenderCommands::ClearTarget(Context, (uint32_t)Fracture::ClearFlags::Color | (uint32_t)Fracture::ClearFlags::Depth);
 
    DrawArray cmd =
    {
        .mode = DrawMode::Triangles,
        .first = 0,
        .count = 3
    };
    RenderCommands::BindVertexArrayObject(Context, GraphicsDevice::Instance()->GetQUADVAO());
    RenderCommands::DrawArray(Context, cmd);

    Fracture::RenderCommands::SetRenderTarget(Context, (uint32_t)0);

}



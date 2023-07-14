#include "FracturePCH.h"
#include "LightProbeSystem.h"
#include "Assets/AssetManager.h"
#include "SceneManager.h"
#include "Rendering/GraphicsDevice.h"


Fracture::LightProbeSystem::LightProbeSystem()
{
    /*
    Fracture::TextureCreationInfo desc;
    desc.Width = 512;
    desc.Height = 512;
    desc.TextureTarget = TextureTarget::TextureCubeMap;
    desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
    desc.format = Fracture::TextureFormat::RGB;
    desc.internalFormat = Fracture::InternalFormat::RGB16F;
    desc.formatType = Fracture::TextureFormatType::Float;
    desc.minFilter = TextureMinFilter::Linear;
    desc.magFilter = TextureMagFilter::Linear;
    desc.Wrap = TextureWrap::ClampToEdge;
    desc.Name = "Global_SkyMap";
    Fracture::GraphicsDevice::Instance()->CreateGlobalTexture("Global_SkyMap", desc);
    */
}


void Fracture::LightProbeSystem::Bake(RenderContext* Context, UUID id)
{
	const auto& lightProbe = SceneManager::GetComponent<LightProbeComponent>(id);
	
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
  
    Fracture::RenderCommands::UseProgram(Context, shader->Handle);
    Fracture::RenderCommands::Disable(Context, Fracture::GLCapability::FaceCulling);
    Fracture::RenderCommands::SetRenderTarget(Context, target->Handle);
    Fracture::RenderCommands::SetRenderBuffer(Context, target->RenderBufferHandle);
    Fracture::RenderCommands::FrameBufferSetDrawBuffers(Context, target->Handle, 1);
    Fracture::RenderCommands::RenderBufferTextureStorage(Context, target->RenderBufferHandle, InternalFormat::DepthComponent24, lightProbe->LightProbeResolution, lightProbe->LightProbeResolution);
  

    Fracture::RenderCommands::SetViewport(Context, lightProbe->LightProbeResolution, lightProbe->LightProbeResolution, 0, 0);
    Fracture::RenderCommands::SetScissor(Context, lightProbe->LightProbeResolution, lightProbe->LightProbeResolution, 0, 0);


    Fracture::RenderCommands::FrameBufferSetDrawBuffers(Context, target->Handle, 1);   
    Fracture::RenderCommands::FrameBufferAttachTexture(Context, target->Handle, 0, GraphicsDevice::Instance()->GetGlobalTexture("Global_SkyMap")->Handle, 0);
    
    
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
}


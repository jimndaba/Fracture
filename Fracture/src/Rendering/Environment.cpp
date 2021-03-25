#pragma once
#include "Environment.h"
#include "RenderTarget.h"
#include "OpenGL/FrameBuffer.h"
#include "OpenGL/RenderBuffer.h"
#include "OpenGL/OpenGLBase.h"
#include "OpenGL/Texture2D.h"
#include "OpenGL/TextureCubeMap.h"
#include "AssetManager/AssetManager.h"
#include "Shader.h"
#include "Model.h"

Fracture::Environment::Environment(const std::string& name, const std::shared_ptr<Shader>& rendershader):
    m_Cubeshader(AssetManager::getShader("CubeMap")),
    m_Rendershader(rendershader),
    m_irradiance(AssetManager::getShader("irradiance")),
    m_prefilter(AssetManager::getShader("prefilter")),
    m_bdrf(AssetManager::getShader("bdrf"))
{
    m_CaptureTarget = RenderTarget::CreateRenderTarget("EnvironmentCapture");
    m_CaptureTarget->GetBuffer()->AddRenderBuffer("EnvironmentBuffer", AttachmentType::Depth, RenderBuffer::CreateBuffer(InternalFormat::DepthComponent24, 512, 512));
}

Fracture::Environment::~Environment()
{
}

void Fracture::Environment::Render(glm::mat4 view, glm::mat4 projection)
{
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    m_Rendershader->use();
    m_Rendershader->setMat4("view", view);
    m_Rendershader->setMat4("projection", projection);
    m_Rendershader->setCubeMap("environmentMap", m_CubeMap->GetTextureID(), 0);  //
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default	
    glDepthMask(GL_TRUE);
}

void Fracture::Environment::RenderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);


}

void Fracture::Environment::RenderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Fracture::Environment::CreateCubeMaptexture()
{
    m_CubeMap = TextureCubeMap::CreateTexture(InternalFormat::RGBA16, TextureFormat::RGB, 512, 512, glWrap::ClampToEdge, FormatType::Float, false);
}

void Fracture::Environment::SetupMatricies()
{
    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
   // ----------------------------------------------------------------------------------------------
    captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    captureViews[0] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    captureViews[1] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    captureViews[2] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    captureViews[3] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    captureViews[4] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
    captureViews[5] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
}

void Fracture::Environment::SetupDynameCubemap()
{
    // pbr: convert HDR equirectangular environment map to cubemap equivalent
    // ----------------------------------------------------------------------
    m_Cubeshader->use();
    //m_Cubeshader->setTexture("equirectangularMap", m_enviroment.get(), 0);
    m_Cubeshader->setMat4("projection", captureProjection);
    m_Cubeshader->setInt("isDynamic", 1);
    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.

    m_CaptureTarget->bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        m_Cubeshader->setMat4("view", captureViews[i]);
        m_CaptureTarget->GetBuffer()->AddAttachment(AttachmentType::Color, 0, AttachmentTarget::CubeMapPosX, i, m_CubeMap->GetTextureID(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderCube();
    }
    m_CaptureTarget->Unbind();
}

void Fracture::Environment::ConvertHDRtoCubeMap()
{
    // pbr: convert HDR equirectangular environment map to cubemap equivalent
    // ----------------------------------------------------------------------
    m_Cubeshader->use();
    m_Cubeshader->setInt("isDynamic",0);
    m_Cubeshader->setTexture("equirectangularMap", m_enviroment.get(), 0);
    m_Cubeshader->setMat4("projection", captureProjection);
    
    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.

    m_CaptureTarget->bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        m_Cubeshader->setMat4("view", captureViews[i]);
        m_CaptureTarget->GetBuffer()->AddAttachment(AttachmentType::Color, 0, AttachmentTarget::CubeMapPosX, i, m_CubeMap->GetTextureID(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderCube();
    }
    m_CaptureTarget->Unbind();
}

void Fracture::Environment::CreateIrradianceMap()
{
    m_IrradianceMap = TextureCubeMap::CreateTexture(InternalFormat::RGBA16, TextureFormat::RGB, 32, 32, glWrap::ClampToEdge, FormatType::Float, false);
    m_IrradianceMap->GenerateMips();
    m_CaptureTarget->bind();
    m_CaptureTarget->GetBuffer()->GetRenderBuffer("EnvironmentBuffer")->Resize(32, 32);

    // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
    // -----------------------------------------------------------------------------
    m_irradiance->use();
    m_irradiance->setCubeMap("environmentMap", m_CubeMap->GetTextureID(), 0);
    m_irradiance->setMat4("projection", captureProjection);

    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.

    for (unsigned int i = 0; i < 6; ++i)
    {
        m_irradiance->setMat4("view", captureViews[i]);
        m_CaptureTarget->GetBuffer()->AddAttachment(AttachmentType::Color, 0, AttachmentTarget::CubeMapPosX, i, m_IrradianceMap->GetTextureID(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        RenderCube();
    }
    m_CaptureTarget->Unbind();
}

void Fracture::Environment::CreatePreFilterMap()
{
    m_PrefilterMap = TextureCubeMap::CreateTexture(InternalFormat::RGBA16, TextureFormat::RGB, 128, 128, glWrap::ClampToEdge, FormatType::Float, true);

    m_PrefilterMap->GenerateMips();

    m_prefilter->use();
    m_prefilter->setCubeMap("environmentMap", m_CubeMap->GetTextureID(), 0);
    m_prefilter->setMat4("projection", captureProjection);

    m_CaptureTarget->bind();
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        uint32_t mipWidth = (int)(128 * std::pow(0.5, mip));
        uint32_t mipHeight = (int)(128 * std::pow(0.5, mip));

        m_CaptureTarget->GetBuffer()->GetRenderBuffer("EnvironmentBuffer")->Resize(mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        m_prefilter->setFloat("roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i)
        {
            m_prefilter->setMat4("view", captureViews[i]);
            m_CaptureTarget->GetBuffer()->AddAttachment(AttachmentType::Color, 0, AttachmentTarget::CubeMapPosX, i, m_PrefilterMap->GetTextureID(), mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            RenderCube();
        }
    }
    m_CaptureTarget->Unbind();
}

void Fracture::Environment::CreateBDRF()
{
    // pre-allocate enough memory for the LUT texture.
    m_bdrfTexture = Texture2D::CreateTexture(InternalFormat::RedGreen16, TextureFormat::RG, 512, 512, glWrap::ClampToEdge, FormatType::Float);

    m_CaptureTarget->bind();
    m_CaptureTarget->GetBuffer()->GetRenderBuffer("EnvironmentBuffer")->Resize(512, 512);
    m_CaptureTarget->GetBuffer()->AddAttachment(AttachmentType::Color, 0, AttachmentTarget::Texture2D, m_bdrfTexture->GetTextureID());
    glViewport(0, 0, 512, 512);
    m_bdrf->use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    RenderQuad();
    m_CaptureTarget->Unbind();
}

std::shared_ptr<Fracture::Shader> Fracture::Environment::GetShader()
{
    return m_Rendershader;
}

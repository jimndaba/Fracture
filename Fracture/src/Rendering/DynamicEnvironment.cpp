#include "DynamicEnvironment.h"
#include "RenderTarget.h"
#include "OpenGL/FrameBuffer.h"
#include "OpenGL/RenderBuffer.h"
#include "OpenGL/OpenGLBase.h"
#include "OpenGL/Texture2D.h"
#include "OpenGL/TextureCubeMap.h"
#include "AssetManager/AssetManager.h"
#include "Shader.h"
#include "Model.h"

Fracture::DynamicEnvironment::DynamicEnvironment(const std::string& name, std::shared_ptr<Shader> mshader):
    Environment(name, mshader),
    mshader(mshader)
{

    CreateCubeMaptexture();

    SetupMatricies();

    SetupDynameCubemap();

    CreateIrradianceMap();

    CreatePreFilterMap();

    CreateBDRF();

}

std::shared_ptr<Fracture::DynamicEnvironment> Fracture::DynamicEnvironment::Create(const std::string& name, std::shared_ptr<Shader> mshader)
{
    return std::make_shared<DynamicEnvironment>(name,mshader);
}

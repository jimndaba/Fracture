#include "StaticEnvironment.h"
#include "RenderTarget.h"
#include "OpenGL/FrameBuffer.h"
#include "OpenGL/RenderBuffer.h"
#include "OpenGL/OpenGLBase.h"
#include "OpenGL/Texture2D.h"
#include "OpenGL/TextureCubeMap.h"
#include "AssetManager/AssetManager.h"
#include "Shader.h"
#include "Model.h"


Fracture::StaticEnvironment::StaticEnvironment(const std::string& name,std::shared_ptr<Texture2D> environment, const std::shared_ptr<Shader>& rendershader):
    Environment(name, rendershader)
{     
    m_enviroment = environment;
   
    CreateCubeMaptexture();
  
    SetupMatricies(); 
    
    ConvertHDRtoCubeMap();
         
    CreateIrradianceMap();
    
    CreatePreFilterMap();

    CreateBDRF();
}

Fracture::StaticEnvironment::~StaticEnvironment()
{
}

std::shared_ptr<Fracture::StaticEnvironment> Fracture::StaticEnvironment::Create(const std::string& name,std::shared_ptr<Texture2D> texture, std::shared_ptr<Shader> shader)
{
	return std::make_shared<StaticEnvironment>(name,texture,shader);
}
